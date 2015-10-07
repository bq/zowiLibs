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
#include <ZowiSerialCommand.h>
#include <EnableInterrupt.h> //Library to manage external interruptions

ZowiSerialCommand SCmd;  // The demo SerialCommand object

//-- Zowi Library
#include <Zowi.h>

Zowi zowi(2,3,4,5);  //-- This is Zowi!!




//-- First step: Configure the pins where the servos are attached
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

#define PIN_YL 2 //servo[0]
#define PIN_YR 3 //servo[1]
#define PIN_RL 4 //servo[2]
#define PIN_RR 5 //servo[3]




//---Zowi Noise Sensor
#define PIN_NoiseSensor A6

//---Zowi Buzzer
#define PIN_Buzzer0 10

//---Zowi Buttons
#define PIN_SecondButton 6
#define PIN_ThirdButton 7



////////////////////////////
// Global Variables       //
////////////////////////////
int trim_YL = 0;  //servo[0]
int trim_YR = 0;  //servo[1]
int trim_RL = 0;  //servo[2]
int trim_RR = 0;  //servo[3]


////////////////////////////
// Setup                  //
////////////////////////////
void setup() {

}



////////////////////////////
// Loop                   //
////////////////////////////
void loop() {

}


