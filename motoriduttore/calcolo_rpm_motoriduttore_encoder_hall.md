## Calcolo velocità di rotazione con encoder meccanico

Spiegazione del calcolo della velocità del motoriduttore in rpm (Rotazioni per minuto) a partire dagli impulsi provenienti dall'encoder calettato sull'albero motore

Piedini di connessione dellìencoder ad effetto Hall a due canali

    #define pin_canaleA  2
    #define pin_canaleB  3

Dichiariamo le variabili necessarie al calcolo:

    volatile int contaImpulsi = 0;

    volatile boolean A = 0;
    volatile boolean B = 0;

Variabili per il conteggio degli impulsi generati in un secondo:

    int  rifImpulsi = 0;
    int deltaImpulsi = 0; 

Variabile per la gestione del tempo:

    long int rifTemporale = 0;

Inseriamo le due routine da eseguire sui cambi di stato dei segnali provenienti dall'encoder:

    void isrA()
    {
     if(digitalRead(pin_canaleB)) 
          conta_impulsi--; //Se B è alto al cambio di A il senso di rotazione è antiorario e diminuisco il numero di impulsi contati 
     else
          conta_impulsi++; //Se B è basso al cambio di A il senso di rotazione è orario e aumento il numero di impulsi contati in avanti
    
    }


### Fase di Setup

    void setup() 
    {
         Serial.begin (9600);
    
         //turn on pullup resistor - le uscite dell'encoder sono open-collector
         
         pinMode(pin_canaleA, INPUT);
         pinMode(pin_canaleB, INPUT);
  
        //Abilito l'interrupt sui pin 2  e dichiaro la funzione da richiamare e quando richiamarle
        
         attachInterrupt(digitalPinToInterrupt(pin_canaleA), isrA, RISING); 

         rifTemporale = millis(); //memorizza il tempo attuale
    }

    

### Fase di loop

         if(millis()-rif_Temporale>=1000){

Ogni secondo che passa calcolo il numero di impulsi aggiuntivi arrivati dall'encoder. 

        deltaImpulsi 

rappresenta il numero di impulsi arrivati nell'ultimo secondo, essendo la differenza tra gli impulsi totali e quelli contati fino al secondo precedente:

        deltaImpulsi = contaImpulsi-rifImpulsi;       //conta gli impulsi nell'ultimo secondo

Aggiorno poi i valori del riferimento temporale e del contegggio.

        rifImpulsi   = contaImpulsi;                 // aggiorno il riferimento di conteggio al valore attuale
        rifTemporale = millis(); }                    //aggiorno il tempo di riferimento
                  
 Ogni 20 impulsi ho un giro del motoriduttore come da prove di lab. Quindi deltaImpulsi/20 è il numero di giri al secondo: 
              
         Serial.print("Giri al secondo: ");  
         Serial.println (deltaImpulsi/20); 



              
   
