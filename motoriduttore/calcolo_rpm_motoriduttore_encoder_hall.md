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
          conta_impulsi--; //Se B è alto al cambio di A il senso di rotazione è antiorario e diminuisco il numero di impulsi contati 
     else
          conta_impulsi++; //Se B è basso al cambio di A il senso di rotazione è orario e aumento il numero di impulsi contati in avanti
    
    }

    void isrB()
    {
         B = !B; //Tiene conto dello stato alto o basso di B aggiornandolo ad ogni transizione (alto-basso o basso-alto)
    }

### Fase di Setup

    void setup() 
    {
         Serial.begin (9600);
    
         //turn on pullup resistor - le uscite dell'encoder sono open-collector
         
         pinMode(pin_canaleA, INPUT_PULLUP);
         pinMode(pin_canaleB, INPUT_PULLUP);
  
        //Abilito gli interrupt sui pin 2 e 3 e dichiaro le funzioni da richiamare e quando richiamarle
        
         attachInterrupt(digitalPinToInterrupt(pin_canaleA), isrA, RISING);
         attachInterrupt(digitalPinToInterrupt(pin_canaleB), isrB, CHANGE); 

         //Salvo i livelli iniziali dei due segnali (alto o basso che siano) sui piedini 2 e 3
         
         A = (boolean)digitalRead(pin_canaleA); //valore iniziale canale A
         B = (boolean)digitalRead(pin_canaleB); //valore iniziale canale B

         tempoIniziale = millis(); //memorizza il tempo attuale
    }

    

### Fase di loop

Ogni secondo che passa calcolo il numero di impulsi aggiuntivi arrivati dall'encoder che misura no il movimento in un secondo:

          if(millis()-rif_Temporale>=1000){
                  deltaImpulsi = contaImpulsi-rifImpulsi;      //conta gli impulsi nell'ultimo secondo
                  rifImpulsi   = contaImpulsi;                 // aggiorno il riferimento di conteggio al valore attuale
                  rifTemporale = millis(); }                    //aggiorno il tempo di riferimento
                  
deltaImpulsi rappresenta il movimento. Aggiorno poi i valori del riferimento temporale e del contegggio. Ogni 60 impulsi ho un giro del motoriduttore come da prove di lab. Quindi deltaImpulsi/60 è il numero di giri al secondo: 
              
              Serial.print("Giri al minuto: ");  
              Serial.println (deltaImpulsi/60 *60); 

Introdurre un delay di 100 ms


              
   
