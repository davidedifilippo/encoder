#define ENCODER_A 2
#define ENCODER_B 3

volatile long count = 0;
unsigned long lastInterruptTime = 0;

void ISR_A() {
  // Debouncing software semplice: ignora impulsi troppo ravvicinati (< 5ms)
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 5) {
    // Se A sale e B è BASSO, va in una direzione
    // Se A sale e B è ALTO, va nell'altra
    if (digitalRead(ENCODER_B) == LOW) {
      count++;
    } else {
      count--;
    }
  }
  lastInterruptTime = interruptTime;
}

void setup() {
  Serial.begin(115200); // 9600 è troppo lento, usa 115200
  
  // Usa le resistenze di pull-up interne
  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), ISR_A, RISING);
}

void loop() {
  static long lastCount = 0;
  if (count != lastCount) {
    Serial.println(count);
    lastCount = count;
  }
}
