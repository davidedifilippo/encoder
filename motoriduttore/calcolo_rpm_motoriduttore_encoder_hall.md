## Calcolo RPM

Spiegazione del calcolo della velocità del motoriduttore in rpm (Rotazioni per minuto) a partire dagli impulsi provenienti dall'encoder calettato sull'albero motore

Piedini di connessione dellìencoder ad effetto Hall a due canali

    #define pin_canaleA  2
    #define pin_canaleB  3

Dichiariamo le variabili necessarie al calcolo:

    volatile int conta_impulsi = 0;

    volatile boolean A = 0;
    volatile boolean B = 0;

Variabili per il conteggio degli impulsi generati in un secondo:

    int Impulsi_iniziali = 0;
    int deltaImpulsi = 0; 

Variabile per la gestione del tempo:

    long int tempoIniziale = 0;

Inseriamo le due routine da eseguire sui cambi di stato dei segnali provenienti dall'encoder:

    void isrA()
    {
     if(B) 
          conta_impulsi--; //Se B è alto al cambio di A il senso di rotazione è antiorario
     else
          conta_impulsi++; //Se B è basso al cambio di A il senso di rotazione è orario
    
    }

    void isrB()
    {
         B = !B; 
    }

### Fase di Setup

    void setup() 
    {
         Serial.begin (9600);
    
         //turn on pullup resistor - le uscite dell'encoder sono open-collector
         
         pinMode(pin_canaleA, INPUT_PULLUP);
         pinMode(pin_canaleB, INPUT_PULLUP);
  
        //Abilito gli interrupt sui pin 2 e 3 sulle variazioni dei segnali e dichiaro le funzioni da richiamare
         attachInterrupt(digitalPinToInterrupt(pin_canaleA), isrA, RISING);
         attachInterrupt(digitalPinToInterrupt(pin_canaleB), isrB, CHANGE); 

         
         A = (boolean)digitalRead(pin_canaleA); //valore iniziale canale A
         B = (boolean)digitalRead(pin_canaleB); //valore iniziale canale B


 

}

### Fase di loop

void loop()
{  
  Serial.print ("Count = ");  
    Serial.println (conta_impulsi);
    delay(10);
}
