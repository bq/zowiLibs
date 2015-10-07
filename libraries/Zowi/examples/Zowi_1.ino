//----------------------------------------------------------------
//-- Zowi: Testing the complete pack
//-- (c) BQ. Released under a GPL licencse
//-- September 2015
//-- Authors:  Javier Isabel:  javier.isabel@bq.com
//--           Juan Gonzalez (obijuan): juan.gonzalez@bq.com
//--           Jose Alberca:   jose.alberca@bq.com
//--           Anita de Prado: ana.deprado@bq.com
//--           Irene Sanz : irene.sanz@bq.com
//-----------------------------------------------------------------
//-- Experiment with all the features that Zowi have!
//-----------------------------------------------------------------

#include <Servo.h>
#include <Oscillator.h>
#include <EEPROM.h>
#include <ZowiSerialCommand.h>
#include <EnableInterrupt.h> //Library to manage external interruptions
#include <US.h>
#include <LedMatrix.h>
#include <BatReader.h>

//-- Zowi Library
#include <Zowi.h>

/*
         --------------- 
        |               |
        |     O   O     |
        |               |
 YR ==> |               | <== YL
         --------------- 
            ||     ||
            ||     ||
            ||     ||
 RR ==>   -----   ------  <== RL
          -----   ------
          */

Zowi zowi;  //-- This is Zowi!!

////////////////////////////
// Global Variables       //
////////////////////////////

////////////////////////////
// Setup                  //
////////////////////////////
void setup() {
  //Serial.begin(9600);
  //tone(10,261,500);
  //delay(500);
  
  //zowi.bendTones (880, 2093, 1.02, 18, 0);
  //zowi.sing(S_surprise);
}



////////////////////////////
// Loop                   //
////////////////////////////
void loop() {
  //zowi.putMouth(0b00000000100001101101010010000000, false);
  
  /*zowi.putMouth(interrogation);
  delay(2000);
  zowi.clearMouth();
  delay(2000);*/

  //zowi.jump();
  /*Serial.println(zowi.getDistance());
  delay(100);*/
}

