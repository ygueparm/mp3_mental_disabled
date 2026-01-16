/*
 * Contrôleur MP3 Minimaliste avec ATtiny85
 * Version: 3.4 FINALE
 * Date: 29 décembre 2025
 * 
 * Spécifications (TESTÉ SUR HARDWARE RÉEL):
 * - Power: Géré par le module MP3 (pas de contrôle depuis ATtiny)
 * - Bouton 1 (PB3): Next Track → PB2 (350ms)
 * - Bouton 2 (PB4): Volume - → PB0 (2500ms)
 * - Setup: Vol+ automatique au démarrage → PB2 (2500ms)
 * - PB0: Volume - uniquement
 * - PB2: Next (350ms) OU Volume+ (2500ms) selon durée
 * - Alimentation 3.3V, CPU 1 MHz
 * - Anti-rebond logiciel 20ms
 * - Temporisations accessibilité: Next=5s, Vol-=2s
 */

// Définition des broches
const uint8_t BTN_NEXT = 3;      // PB3 - Bouton Next Track
const uint8_t BTN_VOL_D = 4;     // PB4 - Bouton Volume -
const uint8_t KEY_VOLDOWN = 0;   // PB0 - Sortie vers KEY_VOLDOWN du MP3 (2500ms)
const uint8_t KEY_TRACK = 2;     // PB2 - Sortie vers KEY (Next 350ms / Vol+ 2500ms)

// Durées de pulse fixes (mesures réelles)
const uint16_t PULSE_NEXT = 250;       // ms - durée pour next track (sur PB2)
const uint16_t PULSE_VOL_UP = 2500;    // ms - durée pour volume + (sur PB2, setup uniquement)
const uint16_t PULSE_VOL_DOWN = 1400;  // ms - durée pour volume - (sur PB0)

// Délai initial pour attendre que le module MP3 démarre
const uint16_t MP3_BOOT_DELAY = 1500;  // ms - attendre que le module MP3 soit prêt

// Constantes de timing
const uint16_t DEBOUNCE_DELAY = 20;    // ms - anti-rebond

// Temporisations anti-appuis multiples (accessibilité)
const uint16_t COOLDOWN_NEXT = 3000;   // ms - 5 secondes entre chaque next
const uint16_t COOLDOWN_VOL_D = 1000;  // ms - 2 secondes entre chaque baisse volume

// Variables d'état Bouton Next
uint8_t lastBtnStateNext = HIGH;
uint32_t lastDebounceTimeNext = 0;
uint8_t btnStateNext = HIGH;
uint32_t lastActionTimeNext = 0;

// Variables d'état Bouton Volume -
uint8_t lastBtnStateVolD = HIGH;
uint32_t lastDebounceTimeVolD = 0;
uint8_t btnStateVolD = HIGH;
uint32_t lastActionTimeVolD = 0;

// Méthode sécurisée pour mettre une broche en sortie open-drain LOW
void setPinLow(uint8_t pin) {
    digitalWrite(pin, LOW);
    pinMode(pin, OUTPUT);
}

// Méthode pour remettre une broche en haute impédance
void setPinHighZ(uint8_t pin) {
    pinMode(pin, INPUT);
    digitalWrite(pin, LOW);
}

// Configure une broche en entrée avec pull-up
void setPinInput(uint8_t pin) {
    pinMode(pin, INPUT_PULLUP);
}

// Génère une impulsion LOW de durée spécifiée sur une broche
void sendPulse(uint8_t pin, uint16_t duration) {
    setPinLow(pin);
    delay(duration);
    setPinHighZ(pin);
}

// Vérifie si un front descendant s'est produit avec anti-rebond
bool buttonFell(uint8_t pin, uint8_t &lastState, uint32_t &lastDebounceTime, uint8_t &currentState) {
    uint8_t reading = digitalRead(pin);
    uint32_t now = millis();
    
    // Si l'état a changé (dû au bruit ou appui réel)
    if (reading != lastState) {
        lastDebounceTime = now; // Réinitialiser le timer de debounce
    }
    
    // Si assez de temps s'est écoulé depuis le dernier changement
    if ((now - lastDebounceTime) > DEBOUNCE_DELAY) {
        // Si l'état est stable et différent de l'état précédent
        if (reading != currentState) {
            currentState = reading;
            
            // Vérifier si c'est un front descendant (HIGH → LOW)
            if (currentState == LOW) {
                lastState = reading;
                return true;
            }
        }
    }
    
    lastState = reading;
    return false;
}

// Vérifie si le délai de cooldown est écoulé depuis la dernière action
bool canTriggerAction(uint32_t &lastActionTime, uint16_t cooldownDelay) {
    uint32_t now = millis();
    
    // Gestion du débordement de millis() (après ~49 jours)
    if (now < lastActionTime) {
        lastActionTime = now;
        return true;
    }
    
    if ((now - lastActionTime) >= cooldownDelay) {
        return true;
    }
    
    return false;
}

void setup() {
    // Configuration des broches d'entrée avec pull-up interne
    setPinInput(BTN_NEXT);
    setPinInput(BTN_VOL_D);
    
    // Configuration initiale des broches de sortie en haute impédance
    setPinHighZ(KEY_VOLDOWN);
    setPinHighZ(KEY_TRACK);
    
    // Lecture initiale des boutons
    lastBtnStateNext = digitalRead(BTN_NEXT);
    lastBtnStateVolD = digitalRead(BTN_VOL_D);
    btnStateNext = lastBtnStateNext;
    btnStateVolD = lastBtnStateVolD;
    
    // Initialiser les timers
    uint32_t now = millis();
    lastDebounceTimeNext = now;
    lastDebounceTimeVolD = now;
    lastActionTimeNext = 0;
    lastActionTimeVolD = 0;
    
    // Attendre que le module MP3 démarre et soit prêt
    delay(MP3_BOOT_DELAY);
    
    // Augmenter le volume 1 fois au démarrage (pulse long 2500ms sur PB2)
    sendPulse(KEY_TRACK, PULSE_VOL_UP);
    
    // Petite pause après l'augmentation du volume
    delay(500);
}

void loop() {
    // Vérifier le bouton Next Track avec temporisation de 5 secondes
    if (buttonFell(BTN_NEXT, lastBtnStateNext, lastDebounceTimeNext, btnStateNext)) {
        if (canTriggerAction(lastActionTimeNext, COOLDOWN_NEXT)) {
            sendPulse(KEY_TRACK, PULSE_NEXT);  // Pulse court 350ms sur PB2 = Next
            lastActionTimeNext = millis();
        }
        // Sinon, l'appui est ignoré (trop rapide)
    }
    
    // Vérifier le bouton Volume - avec temporisation de 2 secondes
    if (buttonFell(BTN_VOL_D, lastBtnStateVolD, lastDebounceTimeVolD, btnStateVolD)) {
        if (canTriggerAction(lastActionTimeVolD, COOLDOWN_VOL_D)) {
            sendPulse(KEY_VOLDOWN, PULSE_VOL_DOWN);  // Pulse long 2500ms sur PB0 = Vol-
            lastActionTimeVolD = millis();
        }
        // Sinon, l'appui est ignoré (trop rapide)
    }
    
    // Petite pause pour éviter la surcharge CPU
    delay(5);
}