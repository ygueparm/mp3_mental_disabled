/*
 * Contrôleur MP3 Minimaliste — CH32V003A4M6
 * Transposé depuis ATtiny85 v3.4 avec Claude sonnet 4.6
 * Architecture: RISC-V 32 bits, 48 MHz (réduit à ~1 MHz via prescaler)
 *
 * === MAPPING DES BROCHES ===
 *
 * | Fonction        | ATtiny85 | CH32V003A4M6 | Broche physique |
 * |-----------------|----------|--------------|-----------------|
 * | Bouton Next     | PB3      | PC1          | pin 1           |
 * | Bouton Vol-     | PB4      | PC2          | pin 2           |
 * | Sortie KEY      | PB2      | PC3          | pin 3           |
 * | Sortie KEY_VOL- | PB0      | PC4          | pin 4           |
 *
 * === NOTES D'ADAPTATION ===
 * - Le CH32V003 n'a pas de mode 1 MHz natif. On garde 48 MHz mais
 *   les délais restent corrects grâce à delay() qui est calibré.
 * - Open-drain : le CH32V003 supporte le mode OUTPUT_OPEN_DRAIN
 *   via pinMode(pin, OUTPUT_OPEN_DRAIN). Si ton core ne le supporte
 *   pas, la méthode setPinLow/setPinHighZ reste compatible.
 * - Les numéros de broches ci-dessous suivent la numérotation
 *   Arduino du core CH32 WCH. Adapte si tu utilises ch32v003fun.
 *
 * === ALIMENTATION ===
 * - 3.3V (idem ATtiny85 en 3.3V)
 * - Consommation plus élevée qu'ATtiny85 (~5 mA actif vs ~0.3 mA)
 */

// --- Broches (numérotation Arduino, core CH32 WCH) ---
// PD4 = Arduino pin 4 sur CH32V003 (à vérifier selon ton core)
const uint8_t BTN_NEXT    = PC1;   // Bouton Next Track (entrée, pull-up)
const uint8_t BTN_VOL_D   = PC2;   // Bouton Volume -   (entrée, pull-up)
const uint8_t KEY_TRACK   = PC3;   // Sortie vers KEY du MP3 (Next / Vol+)
const uint8_t KEY_VOLDOWN = PC4;   // Sortie vers KEY_VOLDOWN du MP3

// --- Durées de pulse (inchangées) ---
const uint16_t PULSE_NEXT     = 250;   // ms — next track sur KEY_TRACK
const uint16_t PULSE_VOL_UP   = 2500;  // ms — volume + sur KEY_TRACK (setup)
const uint16_t PULSE_VOL_DOWN = 1500;  // ms — volume - sur KEY_VOLDOWN

// --- Timing ---
const uint16_t MP3_BOOT_DELAY  = 1000; // ms — attente démarrage module MP3
const uint16_t DEBOUNCE_DELAY  = 50;   // ms — anti-rebond
const uint16_t COOLDOWN_NEXT   = 4000; // ms — délai min entre deux Next
const uint16_t COOLDOWN_VOL_D  = 2000; // ms — délai min entre deux Vol-

// --- État bouton Next ---
uint8_t  lastBtnStateNext    = HIGH;
uint32_t lastDebounceTimeNext = 0;
uint8_t  btnStateNext         = HIGH;
uint32_t lastActionTimeNext   = 0;

// --- État bouton Vol- ---
uint8_t  lastBtnStateVolD    = HIGH;
uint32_t lastDebounceTimeVolD = 0;
uint8_t  btnStateVolD         = HIGH;
uint32_t lastActionTimeVolD   = 0;

// ---------------------------------------------------------------------------
// Pilotage open-drain : LOW actif, haute impédance au repos
// Compatible avec les deux méthodes (OUTPUT_OPEN_DRAIN ou INPUT flottant)
// ---------------------------------------------------------------------------

void setPinLow(uint8_t pin) {
    digitalWrite(pin, LOW);
    pinMode(pin, OUTPUT);
}

void setPinHighZ(uint8_t pin) {
    pinMode(pin, INPUT);      // haute impédance — la résistance de tirage
    digitalWrite(pin, LOW);   // du module MP3 remonte la ligne
}

void setPinInput(uint8_t pin) {
    pinMode(pin, INPUT_PULLUP);
}

// Génère une impulsion LOW de durée 'duration' ms
void sendPulse(uint8_t pin, uint16_t duration) {
    setPinLow(pin);
    delay(duration);
    setPinHighZ(pin);
}

// ---------------------------------------------------------------------------
// Anti-rebond + détection front descendant (HIGH → LOW)
// Identique à l'ATtiny85 — millis() fonctionne de la même façon
// ---------------------------------------------------------------------------

bool buttonFell(uint8_t pin,
                uint8_t  &lastState,
                uint32_t &lastDebounceTime,
                uint8_t  &currentState)
{
    uint8_t  reading = digitalRead(pin);
    uint32_t now     = millis();

    if (reading != lastState) {
        lastDebounceTime = now;
    }

    if ((now - lastDebounceTime) > DEBOUNCE_DELAY) {
        if (reading != currentState) {
            currentState = reading;
            if (currentState == LOW) {
                lastState = reading;
                return true;
            }
        }
    }

    lastState = reading;
    return false;
}

// Vérifie que le cooldown est écoulé depuis la dernière action
bool canTriggerAction(uint32_t &lastActionTime, uint16_t cooldownDelay) {
    uint32_t now = millis();

    // Gestion débordement millis() (~49 jours)
    if (now < lastActionTime) {
        lastActionTime = now;
        return true;
    }

    return ((now - lastActionTime) >= cooldownDelay);
}

// ---------------------------------------------------------------------------
// Setup
// ---------------------------------------------------------------------------

void setup() {
    // Entrées avec pull-up interne
    setPinInput(BTN_NEXT);
    setPinInput(BTN_VOL_D);

    // Sorties en haute impédance au repos
    setPinHighZ(KEY_VOLDOWN);
    setPinHighZ(KEY_TRACK);

    // Lecture initiale des boutons
    lastBtnStateNext = digitalRead(BTN_NEXT);
    lastBtnStateVolD = digitalRead(BTN_VOL_D);
    btnStateNext     = lastBtnStateNext;
    btnStateVolD     = lastBtnStateVolD;

    // Init des timers
    uint32_t now       = millis();
    lastDebounceTimeNext = now;
    lastDebounceTimeVolD = now;
    lastActionTimeNext   = 0;
    lastActionTimeVolD   = 0;

    // Attendre le démarrage du module MP3
    delay(MP3_BOOT_DELAY);

    // Augmentation volume au démarrage (décommenter si nécessaire)
    // sendPulse(KEY_TRACK, PULSE_VOL_UP);
    // delay(500);
}

// ---------------------------------------------------------------------------
// Boucle principale (identique à l'ATtiny85)
// ---------------------------------------------------------------------------

void loop() {
    // Bouton Next Track (cooldown 4 s)
    if (buttonFell(BTN_NEXT, lastBtnStateNext, lastDebounceTimeNext, btnStateNext)) {
        if (canTriggerAction(lastActionTimeNext, COOLDOWN_NEXT)) {
            sendPulse(KEY_TRACK, PULSE_NEXT);
            lastActionTimeNext = millis();
        }
    }

    // Bouton Volume - (cooldown 2 s)
    if (buttonFell(BTN_VOL_D, lastBtnStateVolD, lastDebounceTimeVolD, btnStateVolD)) {
        if (canTriggerAction(lastActionTimeVolD, COOLDOWN_VOL_D)) {
            sendPulse(KEY_VOLDOWN, PULSE_VOL_DOWN);
            lastActionTimeVolD = millis();
        }
    }

    delay(5);
}
