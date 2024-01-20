#include <Arduino.h>

//PIN's definition

#define pin_canaleA  2
#define pin_canaleB  3

volatile int conta_impulsi = 0;

volatile boolean A = 0;
volatile boolean B = 0;

void doEncoderA()
{
     if(B) 
          conta_impulsi--; //Se B è alto al cambio di A il senso di rotazione è antiorario
     else
          conta_impulsi++; //Se B è basso al cambio di A il senso di rotazione è orario
    
}

void doEncoderB()
{
     B = !B; 
}

void setup() 
{
  Serial.begin (9600);
   
   //turn on pullup resistor
  pinMode(pin_canaleA, INPUT_PULLUP);
  pinMode(pin_canaleB, INPUT_PULLUP);
  


  A = (boolean)digitalRead(pin_canaleA); //valore iniziale canale A
  B = (boolean)digitalRead(pin_canaleB); //valore iniziale canale B


  attachInterrupt(digitalPinToInterrupt(pin_canaleA), doEncoderA, RISING);

  attachInterrupt(digitalPinToInterrupt(pin_canaleB), doEncoderB, CHANGE); 

}


void loop()
{  
  Serial.print ("Count = ");  
    Serial.println (conta_impulsi);
    delay(10);
}

