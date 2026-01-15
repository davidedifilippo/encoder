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

void ISR_A() {
  // Debouncing software: ignora impulsi troppo ravvicinati (< 5ms)
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 5) {
    
    // Controllo la direzione
    if (digitalRead(B_CHANNEL) == LOW) {
      if (count < 4095) count++; // Incrementa solo se sotto il massimo
    } else {
      if (count > 0) count--;    // Decrementa solo se sopra il minimo
    }
  }
  lastInterruptTime = interruptTime;
}

void setup() {
  Serial.begin(115200);

  // Configurazione Pin Encoder
  pinMode(A_CHANNEL, INPUT);
  pinMode(B_CHANNEL, INPUT);
  attachInterrupt(digitalPinToInterrupt(A_CHANNEL), ISR_A, RISING);

  // Configurazione DAC
  analogWriteResolution(12); // Imposta la risoluzione a 12 bit (0-4095)
  
  Serial.println("--- Sistema di controllo DAC con Encoder ---");
  Serial.println("Ruota l'encoder per variare la tensione su pin A0 (DAC)");
}

void loop() {
  // Verifichiamo se il conteggio è cambiato
  if (count != lastCount) {
    
    // Aggiorno l'uscita del DAC sul pin A0
    analogWrite(DAC, count);
    
    // Calcolo la tensione teorica per il feedback seriale
    float voltage = (count * 5.0) / 4095.0;

    // Output su Serial Monitor
    Serial.print("Valore DAC: ");
    Serial.print(count);
    Serial.print(" | Tensione stimata: ");
    Serial.print(voltage, 3);
    Serial.println(" V");

    lastCount = count;
  }
}
