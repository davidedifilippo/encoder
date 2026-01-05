#define ENCODER_A 2
#define ENCODER_B 4
#define STEP 5
#define DIR 6
#define ENABLE 7

volatile long motorPosition = 0;

//Se il motore ruota in senso orario i segnali A e B assumono valori diversi dopo il fronte di salita del segnale A
//Se il motore ruota in senso antiorario i segnali A e B assumono lo stesso valore dopo il fronte di salita di A

void updateMotorPosition(){
  if(digitalRead(ENCODER_B) != digitalRead(ENCODER_A)){
    motorPosition ++;
  }
  else{
    motorPosition --;
  }
}


void setup() {
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), updateMotorPosition, CHANGE);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT); digitalWrite(DIR, HIGH);
  pinMode(ENABLE, OUTPUT); digitalWrite(ENABLE, HIGH);
}

void loop() {
  digitalWrite(STEP, LOW); delayMicroseconds(50);
  digitalWrite(STEP, HIGH); delay(50);
  Serial.println(motorPosition);
}
