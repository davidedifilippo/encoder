/*
   Regolatore di tensione tramite Encoder EC11 e DAC
   Scheda: Arduino Nano R4 (Minima o WiFi)
*/

#define A_CHANNEL 2
#define B_CHANNEL 3

// Variabili per l'encoder
volatile int count = 0; // Usiamo int perché il range 0-4095 rientra comodamente in 16 bit
unsigned long lastInterruptTime = 0;
int lastCount = -1; // Inizializzato a -1 per forzare il primo aggiornamento
int valueToShow = 0;


// --- CONFIGURAZIONE PIN ---
// Pin per i 4 bit del decoder BCD (A, B, C, D)
const int pinBCD[] = {5, 6, 7, 8}; 


// Pin per attivare i transistor delle 4 cifre (DIG1, DIG2, DIG3, DIG4)
const int pinDigits[] = {9, 10, 11, 12};

// Pin per il punto decimale 
const int pinDP = 13; 

// Pin del DAC (Arduino Nano R4)
const int pinDAC = A0;

// --- VARIABILI GLOBALI ---
unsigned long previousMicros = 0;
unsigned long previous = 0;
const long interval = 5000; // Tempo di accensione per ogni cifra (3ms = 3000µs)
int currentDigit = 0;       // Indice della cifra attiva corrente (0-3)

int dacValue = 0;           // Valore da inviare al DAC (0-4095)
int dacDirection = 1;      // Velocità di cambio tensione (per l'effetto demo)

// Buffer per memorizzare le 4 cifre da visualizzare
int displayBuffer[4] = {0, 0, 0, 0};


void setup() {
 

   // Configura i pin BCD come output
  for (int i = 0; i < 4; i++) {
    pinMode(pinBCD[i], OUTPUT);
  }
  
  // Configura i pin delle cifre come output
  for (int i = 0; i < 4; i++) {
    pinMode(pinDigits[i], OUTPUT);
    digitalWrite(pinDigits[i], HIGH); // Spegni tutto all'inizio
  }

  //pinMode(pinDP, OUTPUT);
  
  //Aumenta la risoluzione del DAC a 12 bit
  analogWriteResolution(12); 

  // Configurazione Pin Encoder
  pinMode(A_CHANNEL, INPUT);
  pinMode(B_CHANNEL, INPUT);
  attachInterrupt(digitalPinToInterrupt(A_CHANNEL), ISR_A, RISING);

  // Configurazione DAC
  analogWriteResolution(12); // Imposta la risoluzione a 12 bit (0-4095)
  

}

void loop() {
  // Verifichiamo se il conteggio è cambiato
  if (count != lastCount) {
    
    // Aggiorno l'uscita del DAC sul pin A0
    analogWrite(DAC, count);
    
    // Calcolo la tensione teorica per il feedback seriale
    float voltage = (count * 5.0) / 4095.0;

    // Output su Serial Monitor
   // 3. PREPARAZIONE DATI PER IL DISPLAY
  // Vogliamo mostrare es: 3.145 -> Convertiamo in intero 3145
  valueToShow = (int)(voltage * 1000); 

    lastCount = count;
  }

  displayBuffer[0] = (valueToShow / 1000) % 10; // Cifra più significativa (Volt)
  displayBuffer[1] = (valueToShow / 100) % 10;
  displayBuffer[2] = (valueToShow / 10) % 10;
  displayBuffer[3] = valueToShow % 10;          // Cifra meno significativa

  // 4. GESTIONE MULTIPLEXING (Non bloccante)
  unsigned long currentMicros = micros();
  if (currentMicros - previousMicros >= interval) {
    previousMicros = currentMicros;
    updateDisplay();
  }
}

void ISR_A() {
  // Debouncing software: ignora impulsi troppo ravvicinati (< 5ms)
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 5) {
    
    // Controllo la direzione
    if (digitalRead(B_CHANNEL) == LOW) {
      if (count < 4095) count = count +50; // Incrementa solo se sotto il massimo
    } else {
      if (count > 0) count = count - 50;    // Decrementa solo se sopra il minimo
    }
  }
  lastInterruptTime = interruptTime;
}


// Funzione che aggiorna una singola cifra alla volta
void updateDisplay() {
  // 1. Spegni tutte le cifre (per evitare ghosting)
  for (int i = 0; i < 4; i++) {
    // Se usi transistor NPN: LOW spegne. Se PNP: HIGH spegne.
    // Qui assumo NPN (attivo alto), quindi LOW per spegnere.
    digitalWrite(pinDigits[i], HIGH); 
  }

  // 2. Scrivi il valore BCD della cifra corrente
  int digitValue = displayBuffer[currentDigit];
  // Scrivi i 4 bit: A, B, C, D (LSB to MSB)
  digitalWrite(pinBCD[0], (digitValue & 0x01));      // Bit 0
  digitalWrite(pinBCD[1], (digitValue & 0x02) >> 1); // Bit 1
  digitalWrite(pinBCD[2], (digitValue & 0x04) >> 2); // Bit 2
  digitalWrite(pinBCD[3], (digitValue & 0x08) >> 3); // Bit 3

  // 3. Gestione Punto Decimale
  // Accendiamo il punto solo se siamo sulla prima cifra (indice 0) per avere "X.XXX"
  if (currentDigit == 0) {
    digitalWrite(pinDP, HIGH); // O LOW se il display è a anodo comune
  } else {
    digitalWrite(pinDP, LOW);
  }

  // 4. Accendi la cifra corrente
  // Assumo transistor NPN (HIGH per accendere). Inverti se usi PNP.
  digitalWrite(pinDigits[currentDigit], LOW);

  // 5. Passa alla prossima cifra
  currentDigit++;
  if (currentDigit > 3) currentDigit = 0;
}
