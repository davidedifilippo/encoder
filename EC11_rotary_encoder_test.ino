#define A_CHANNEL 2
#define B_CHANNEL 3

volatile long count = 0;
unsigned long lastInterruptTime = 0;

long lastCount = 0;

void ISR_A() {
  // Debouncing software semplice: ignora impulsi troppo ravvicinati (< 5ms)
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 5) {
    // Se A sale e B è BASSO, va in una direzione
    // Se A sale e B è ALTO, va nell'altra
    
    if (digitalRead(B_CHANNEL) == LOW) {
      count++;
    } else {
      count--;
    }
  }
  lastInterruptTime = interruptTime;
}

void setup() {
  Serial.begin(115200); 
  
  // Imposto i piedini 
  pinMode(A_CHANNEL, INPUT);
  pinMode(B_CHANNEL, INPUT);

   //Attivo l'interrupt sul piedino 2
  //Un fronte di salita sul canale A dell'encoder innesca la routine ISR_A
  attachInterrupt(digitalPinToInterrupt(A_CHANNEL), ISR_A, RISING); 
}

void loop() {
  
  //Stampo solo se ho un incremento/decremento

  if (count != lastCount) {
    Serial.println(count);
    lastCount = count;
  }
}
