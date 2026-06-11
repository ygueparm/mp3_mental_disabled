/*
 * Contrôleur MP3 Minimaliste — STM8S001J3
 * Porté depuis ATtiny85 v3.4
 *
 * Toolchain : SDCC + stm8flash
 * Compilé avec : sdcc -mstm8 --std-sdcc11 main.c
 * Flashé avec  : stm8flash -c stlinkv2 -p stm8s001j3 -w main.ihx
 *
 * Mapping des broches (boîtier SO8) :
 *  PD5 (pin 1) → BTN_NEXT    (entrée, pull-up interne)
 *  PA1 (pin 2) → non utilisé  (sortie push-pull, forcée à 0)
 *  PA2 (pin 3) → non utilisé  (sortie push-pull, forcée à 0)
 *  PA3 (pin 4) → non utilisé  (sortie push-pull, forcée à 0)
 *  PC3 (pin 5) → KEY_VOLDOWN (sortie open-drain)
 *  PC4 (pin 6) → KEY_TRACK   (sortie open-drain)
 *  PC5 (pin 7) → BTN_VOL_D   (entrée, pull-up interne) ← SWIM libre sur PD1
 *  PD6 (pin 8) → non utilisé  (sortie push-pull, forcée à 0)
 *
 * Horloge : HSI 16 MHz / 32 = 500 kHz (CLK_CKDIVR = 0x1B)
 *
 * Timer : TIM4 génère une interruption toutes les 1 ms
 *         → tick_ms est incrémenté UNIQUEMENT dans l'ISR, jamais dans delay_ms.
 *         → millis() et les cooldowns sont donc exacts même pendant send_pulse.
 */

#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Registres STM8S001J3                                                */
/* ------------------------------------------------------------------ */

/* Horloge */
#define CLK_CKDIVR   (*(volatile uint8_t*)0x50C6)

/* Port A */
#define PA_ODR       (*(volatile uint8_t*)0x5000)
#define PA_DDR       (*(volatile uint8_t*)0x5002)
#define PA_CR1       (*(volatile uint8_t*)0x5003)
#define PA_CR2       (*(volatile uint8_t*)0x5004)

/* Port C */
#define PC_IDR       (*(volatile uint8_t*)0x500B)
#define PC_ODR       (*(volatile uint8_t*)0x500A)
#define PC_DDR       (*(volatile uint8_t*)0x500C)
#define PC_CR1       (*(volatile uint8_t*)0x500D)
#define PC_CR2       (*(volatile uint8_t*)0x500E)

/* Port D */
#define PD_IDR       (*(volatile uint8_t*)0x5011)
#define PD_ODR       (*(volatile uint8_t*)0x5010)
#define PD_DDR       (*(volatile uint8_t*)0x5013)
#define PD_CR1       (*(volatile uint8_t*)0x5014)
#define PD_CR2       (*(volatile uint8_t*)0x5015)

/*
 * TIM4 — timer 8 bits, parfait pour timebase 1 ms
 *   TIM4_CR1  : contrôle (bit 0 = CEN, active le compteur)
 *   TIM4_IER  : interrupt enable (bit 0 = UIE, débordement)
 *   TIM4_SR   : status (bit 0 = UIF, flag débordement)
 *   TIM4_PSCR : prescaler (valeur = 2^N)
 *   TIM4_ARR  : auto-reload (valeur max du compteur)
 */
#define TIM4_CR1     (*(volatile uint8_t*)0x5340)
#define TIM4_IER     (*(volatile uint8_t*)0x5343)
#define TIM4_SR      (*(volatile uint8_t*)0x5344)
#define TIM4_PSCR    (*(volatile uint8_t*)0x5347)
#define TIM4_ARR     (*(volatile uint8_t*)0x5348)

/* Interruptions */
#define enableInterrupts()  __asm__("rim")
#define disableInterrupts() __asm__("sim")

/* ------------------------------------------------------------------ */
/* Broches                                                             */
/* ------------------------------------------------------------------ */

#define BTN_NEXT_BIT     5   /* PD5 — bouton Next Track  */
#define BTN_VOLD_BIT     5   /* PC5 — bouton Volume -    */
#define KEY_VOLDOWN_BIT  3   /* PC3 — sortie vol-        */
#define KEY_TRACK_BIT    4   /* PC4 — sortie next/vol+   */

#define BTN_NEXT_READ()   ((PD_IDR >> BTN_NEXT_BIT) & 1)
#define BTN_VOLD_READ()   ((PC_IDR >> BTN_VOLD_BIT) & 1)

/* ------------------------------------------------------------------ */
/* Timebase — TIM4 ISR                                                 */
/* ------------------------------------------------------------------ */

static volatile uint32_t tick_ms = 0;

/*
 * ISR TIM4 — vecteur 23 sur STM8S
 */
void TIM4_UPD_OVF_IRQHandler(void) __interrupt(23)
{
    TIM4_SR &= ~0x01;   /* acquitter le flag UIF */
    tick_ms++;
}

/*
 * Lecture atomique de tick_ms (IRQ coupées le temps de lire les 4 octets,
 * indispensable sur architecture 8 bits).
 */
static uint32_t millis(void)
{
    uint32_t val;
    disableInterrupts();
    val = tick_ms;
    enableInterrupts();
    return val;
}

/* ------------------------------------------------------------------ */
/* Délai bloquant — tick_ms avance via l'ISR, pas ici                 */
/* ------------------------------------------------------------------ */

static void delay_ms(uint16_t ms)
{
    uint32_t start = millis();
    while ((millis() - start) < ms) {
        /* attente active */
    }
}

/* ------------------------------------------------------------------ */
/* Durées                                                              */
/* ------------------------------------------------------------------ */

#define PULSE_NEXT       250U
#define PULSE_VOL_UP    2500U
#define PULSE_VOL_DOWN  1500U
#define MP3_BOOT_DELAY  1000U
#define DEBOUNCE_DELAY    50U
#define COOLDOWN_NEXT   4000U
#define COOLDOWN_VOLD   2000U

/* ------------------------------------------------------------------ */
/* État des boutons                                                    */
/* ------------------------------------------------------------------ */

static uint8_t  lastStateNext  = 1;
static uint8_t  btnStateNext   = 1;
static uint32_t debounceNext   = 0;
static uint32_t lastActionNext = 0;

static uint8_t  lastStateVolD  = 1;
static uint8_t  btnStateVolD   = 1;
static uint32_t debounceVolD   = 0;
static uint32_t lastActionVolD = 0;

/* ------------------------------------------------------------------ */
/* Helpers open-drain                                                  */
/* ------------------------------------------------------------------ */

static void pin_low(uint8_t is_track)
{
    if (is_track) {
        PC_ODR &= ~(1 << KEY_TRACK_BIT);
        PC_DDR |=  (1 << KEY_TRACK_BIT);
    } else {
        PC_ODR &= ~(1 << KEY_VOLDOWN_BIT);
        PC_DDR |=  (1 << KEY_VOLDOWN_BIT);
    }
}

static void pin_hiz(uint8_t is_track)
{
    if (is_track) {
        PC_DDR &= ~(1 << KEY_TRACK_BIT);
        PC_CR1 &= ~(1 << KEY_TRACK_BIT);
    } else {
        PC_DDR &= ~(1 << KEY_VOLDOWN_BIT);
        PC_CR1 &= ~(1 << KEY_VOLDOWN_BIT);
    }
}

static void send_pulse(uint8_t is_track, uint16_t duration_ms)
{
    pin_low(is_track);
    delay_ms(duration_ms);
    pin_hiz(is_track);
}

/* ------------------------------------------------------------------ */
/* Détection front descendant avec anti-rebond                        */
/* ------------------------------------------------------------------ */

static uint8_t button_fell(uint8_t reading,
                            uint8_t  *lastState,
                            uint32_t *debounceTime,
                            uint8_t  *currentState)
{
    uint32_t now = millis();

    if (reading != *lastState)
        *debounceTime = now;

    /* Soustraction non signée : correct même en cas de débordement 32 bits */
    if ((now - *debounceTime) > DEBOUNCE_DELAY) {
        if (reading != *currentState) {
            *currentState = reading;
            if (*currentState == 0) {
                *lastState = reading;
                return 1;
            }
        }
    }

    *lastState = reading;
    return 0;
}

/* ------------------------------------------------------------------ */
/* Vérification cooldown                                               */
/* ------------------------------------------------------------------ */

static uint8_t can_trigger(uint32_t *lastActionTime, uint16_t cooldown)
{
    /* Soustraction non signée : robuste au débordement */
    if ((millis() - *lastActionTime) >= cooldown)
        return 1;
    return 0;
}

/* ------------------------------------------------------------------ */
/* Initialisation TIM4 pour 1 ms à 500 kHz                            */
/*                                                                     */
/* F_CPU = 500 000 Hz                                                  */
/* Prescaler /8  → F_tim = 62 500 Hz → période = 16 µs               */
/* ARR = 62 → débordement toutes les 63 × 16 µs = 1,008 ms ≈ 1 ms    */
/* ------------------------------------------------------------------ */

static void tim4_init(void)
{
    TIM4_CR1  = 0x00;
    TIM4_PSCR = 0x03;   /* prescaler /8 (2^3)           */
    TIM4_ARR  = 62;     /* auto-reload : compte de 0→62 */
    TIM4_IER  = 0x01;   /* interruption débordement     */
    TIM4_SR   = 0x00;   /* acquitter flag résiduel      */
    TIM4_CR1  = 0x01;   /* démarrer (CEN=1)             */
}

/* ------------------------------------------------------------------ */
/* Initialisation matérielle                                           */
/* ------------------------------------------------------------------ */

static void hw_init(void)
{
    /* Horloge : HSI/32 = 500 kHz */
    CLK_CKDIVR = 0x1B;

    /* ----------------------------------------------------------------
     * Broches non utilisées → sortie push-pull forcée à 0
     * (évite les commutations parasites et la consommation associée)
     * PA1, PA2, PA3 : DDR=1 (sortie), CR1=1 (push-pull), ODR=0
     * PD6           : DDR=1 (sortie), CR1=1 (push-pull), ODR=0
     * ---------------------------------------------------------------- */
    PA_ODR &= ~((1 << 1) | (1 << 2) | (1 << 3));
    PA_DDR |=   (1 << 1) | (1 << 2) | (1 << 3);
    PA_CR1 |=   (1 << 1) | (1 << 2) | (1 << 3);
    PA_CR2 &=  ~((1 << 1) | (1 << 2) | (1 << 3));  /* pas d'IRQ externe */

    PD_ODR &= ~(1 << 6);
    PD_DDR |=  (1 << 6);
    PD_CR1 |=  (1 << 6);
    PD_CR2 &= ~(1 << 6);

    /* ----------------------------------------------------------------
     * PD5 : BTN_NEXT — entrée avec pull-up interne
     * DDR=0 (entrée), CR1=1 (pull-up), CR2=0 (pas d'IRQ)
     * ---------------------------------------------------------------- */
    PD_DDR &= ~(1 << BTN_NEXT_BIT);
    PD_CR1 |=  (1 << BTN_NEXT_BIT);
    PD_CR2 &= ~(1 << BTN_NEXT_BIT);

    /* ----------------------------------------------------------------
     * PC5 : BTN_VOLD — entrée avec pull-up interne
     * PC3, PC4 : open-drain au repos (haute impédance, DDR=0, CR1=0)
     * ---------------------------------------------------------------- */
    PC_DDR &= ~((1 << BTN_VOLD_BIT) | (1 << KEY_VOLDOWN_BIT) | (1 << KEY_TRACK_BIT));
    PC_CR1 |=  (1 << BTN_VOLD_BIT);
    PC_CR1 &= ~((1 << KEY_VOLDOWN_BIT) | (1 << KEY_TRACK_BIT));
    PC_CR2 &= ~((1 << BTN_VOLD_BIT) | (1 << KEY_VOLDOWN_BIT) | (1 << KEY_TRACK_BIT));

    /* Timer 4 */
    tim4_init();

    /* Activer les interruptions globales */
    enableInterrupts();
}

/* ------------------------------------------------------------------ */
/* Programme principal                                                 */
/* ------------------------------------------------------------------ */

void main(void)
{
    hw_init();

    lastStateNext = BTN_NEXT_READ();
    btnStateNext  = lastStateNext;
    lastStateVolD = BTN_VOLD_READ();
    btnStateVolD  = lastStateVolD;

    debounceNext   = millis();
    debounceVolD   = millis();
    lastActionNext = 0;
    lastActionVolD = 0;

    delay_ms(MP3_BOOT_DELAY);

    /* Volume+ au démarrage — décommentez si nécessaire */
    /* send_pulse(1, PULSE_VOL_UP); */
    /* delay_ms(500); */

    for (;;) {
        /* Bouton Next Track (PD5) */
        if (button_fell(BTN_NEXT_READ(),
                        &lastStateNext, &debounceNext, &btnStateNext)) {
            if (can_trigger(&lastActionNext, COOLDOWN_NEXT)) {
                send_pulse(1, PULSE_NEXT);
                lastActionNext = millis();
            }
        }

        /* Bouton Volume - (PC5) */
        if (button_fell(BTN_VOLD_READ(),
                        &lastStateVolD, &debounceVolD, &btnStateVolD)) {
            if (can_trigger(&lastActionVolD, COOLDOWN_VOLD)) {
                send_pulse(0, PULSE_VOL_DOWN);
                lastActionVolD = millis();
            }
        }

        delay_ms(5);
    }
}
