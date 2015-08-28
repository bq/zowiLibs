



//----------------------------------------------------------------
//-- Zowi: Testing the complete pack
//-- (c) BQ. Released under a GPL licencse
//-- August 2015
//-- Authors:  Javier Isabel:  javier.isabel@bq.com
//--           Juan Gonzalez (obijuan): juan.gonzalez@bq.com
//--           Jose Alberca: jose.alberca@bq.com
//--           Anita de Prado: ana.deprado@bq.com
//-----------------------------------------------------------------
//-- Experiment with all the features that Zowi have!
//-----------------------------------------------------------------

#include <Servo.h>
#include <Oscillator.h>
#include <EEPROM.h>
#include <ZowiSerialCommand.h>
#include <EnableInterrupt.h> //Library to manage external interruptions

ZowiSerialCommand SCmd;  // The demo SerialCommand object

//-- Zowi Library
#include <Zowi.h>

Zowi zowi;  //-- This is Zowi!!

//---Zowi Led Array Mouth
#include <LedMatrix.h>
#include <Zowi_mouths.h>

LedMatrix ledmatrix(11, 13, 12);


//---Zowi Battery reader library
#include <BatReader.h>

BatReader battery;




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

          #define PIN_RR 4
          #define PIN_RL 5
          #define PIN_YR 2
          #define PIN_YL 3

//-- Second step:  Determine the Servo Trim values
//EVT3
// #define TRIM_RR   -2
// #define TRIM_RL   -9
// #define TRIM_YR   4
// #define TRIM_YL  0


#define TRIM_RR   8
#define TRIM_RL   -22
#define TRIM_YR   4
#define TRIM_YL  6

/*
//EVT2-1
#define TRIM_RR   -20
#define TRIM_RL   -18
#define TRIM_YR   -0
#define TRIM_YL  3
*/



//bqBAT
long TP_init(int trigger_pin, int echo_pin);
long Distance(int trigger_pin, int echo_pin);
#define PIN_TriggerBat 8
#define PIN_EchoBat 9

//Noise Sensor
#define PIN_NoiseSensor A6

//Buzzer
#define PIN_Buzzer 10

//Buttons
#define PIN_SecondButton 6
#define PIN_ThirdButton 7



////////////////////////////
// Global Variables       //
////////////////////////////

int programID=1; //Each program will have a ID in order to 
int T=1000; //Initial duration of movement
int moveId=0; //Number of movement
int endmove=1; // 1= stop 0= moving
int moveSize=15; //Many movements accept a moveSize parameter asociated with the height of that movement
int surprised=0; //For US Test
volatile int state=0; //State of zowi state machine
volatile int buttonPushed=0; //Variable to remember when a button has been pushed
unsigned long previousMillis=0;
int sensorFeedbackOn=0; //Variable to activate the sensor comunication from Zowi to the App


////////////////////////////
// Setup                  //
////////////////////////////

void setup() {

  //Serial communication initialization
  Serial.begin(115200);  
  
  //PinMode of the components
  pinMode(PIN_EchoBat, INPUT );
  pinMode(PIN_TriggerBat, OUTPUT );
  pinMode(PIN_Buzzer,OUTPUT);
  pinMode(PIN_SecondButton,INPUT);
  pinMode(PIN_ThirdButton,INPUT);
  
  //Set the servo pins
  zowi.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR,true);


  //Uncomment this to set the servo trims manually and save on EEPROM 
  //zowi.setTrims(TRIM_YL, TRIM_YR, TRIM_RL, TRIM_RR);
  //zowi.saveTrimsOnEEPROM(); //Uncomment this only for one upload when you finaly set the trims.


  //RandomSeed
  randomSeed (analogRead(PIN_NoiseSensor));

  //Interrumptions
  enableInterrupt(PIN_SecondButton, secondButtonPushed, CHANGE);
  enableInterrupt(PIN_ThirdButton, thirdButtonPushed, CHANGE);

  // Setup callbacks for SerialCommand commands 
  SCmd.addCommand("M", receiveMovement);  //  sendAck();
  SCmd.addCommand("S", stopp);            //  sendAck();
  SCmd.addCommand("L", receiveLED);       //  sendAck();
  SCmd.addCommand("T", recieveBuzzer);    //  sendAck();
  SCmd.addCommand("D", requestDistance);
  SCmd.addCommand("N", requestNoise);
  SCmd.addCommand("B", requestBattery);
  SCmd.addCommand("I", requestProgramId);
  SCmd.addDefaultHandler(stopp);


  //-- Zowi wake up!
  //A little moment of initial surprise
  ledmatrix.writeFull(mouth[bigSurprise]);
  zowi.jump(1,700);

  //Zowi's resting position
  zowi.home();
  
  //Big Smile for a happy Zowi
  ledmatrix.writeFull(mouth[happyOpenMouth]);

}



///**PRUEBAS SONIDOOOOO
int prueba_sonido=0;
#define note_E6 1318.51  //E6
#define note_E7 2637.02  //E7
#define note_A7 3520  //A7

//**MELODIES***
float conexionNotes[]={
    note_E6,50,30,
    note_E7,50,30,
    note_A7,50,30
};


////////////////////////////
// Loop                   //
////////////////////////////
void loop() {

      //pruebas sonidoooo
      if(prueba_sonido==1){
          
          int sssize = (sizeof(conexionNotes)/sizeof(float));
          ZowiMelody(conexionNotes,sssize);
          delay(2000);

          //desconexion();
          delay(2000);

          int x=3135;
          for (int i=1760; i<33135; i=i*1.02) {                        
              
              x=x/1.02; 
              ZowiTone(PIN_Buzzer,x,10,0);
              ZowiTone(PIN_Buzzer,i,10,i/250);

            }

            delay(10000);
      }



  //First attemp to initial software
  if (buttonPushed)
  {  
    //ledmatrix.writeFull(mouth[state]);
    zowi.home();
    delay(500);
    buttonPushed=0;
    ledmatrix.writeFull(mouth[happyOpenMouth]);
    
  }


  if  (Serial.available()>0 && state!=4)
  {
    state=4;
    ledmatrix.writeFull(mouth[state]);
    zowi.home();
    delay(500);
    ledmatrix.writeFull(mouth[happyOpenMouth]);
  }


  if(state==0) //Wait
  {
    ledmatrix.writeFull(mouth[happyOpenMouth]);
    if (millis()-previousMillis>=8000)
    {
      ledmatrix.writeFull(mouth[smallSurprise]);
      zowi.swing(2,800,20);    
      previousMillis=millis();
      zowi.home();
      
    }
      if (Distance(PIN_TriggerBat,PIN_EchoBat)<=15)
      {       
        ledmatrix.writeFull(mouth[bigSurprise]);
        zowi.jump(1, 500);

        while(Distance(PIN_TriggerBat,PIN_EchoBat)<=15){
          ledmatrix.writeFull(mouth[heart]);
          zowi.crusaito(1,1000,20,1);
          if(buttonPushed){break;}  
        }

        zowi.home();
        
      }
  }

  else if (state==1) //DANCE STATE!!
  {
    int randomDance=random(5,20);
    int randomSteps=random(3,6);
    ledmatrix.writeFull(mouth[random(10,21)]);

    for (int i=0;i<randomSteps;i++)
    {
      move(randomDance);
      if(buttonPushed){break;}
    }
  }

  else if (state==2) //Obstacle detector mode
  {
    zowi.walk(1,1000,1);
    surprised=0;
    ledmatrix.writeFull(mouth[happyOpenMouth]);

    if (Distance(PIN_TriggerBat,PIN_EchoBat)<=15)
    { 
      
      ledmatrix.writeFull(mouth[bigSurprise]);
      zowi.jump(5, 500);
      
      if(!buttonPushed)
      {
        ledmatrix.writeFull(mouth[confused]);
        for(int i=0;i<3;i++)
        {
          zowi.walk(1,1000,-1);
          if(buttonPushed){break;}
        } 
        for (int i=0;i<4;i++){
          zowi.turn(1,1000,1);
          if(buttonPushed){break;}
        }
      }
      
    }
  } 

  else if (state==3)  //Noise sensor mode
  {  
    if (analogRead(PIN_NoiseSensor)>=740)
    {
      
      ledmatrix.writeFull(mouth[bigSurprise]);
      delay(400);
      
      if(!buttonPushed){

        ledmatrix.writeFull(mouth[random(10,21)]);
        move(random(1,20));
        zowi.home();
        delay(500);
      }
      
      ledmatrix.writeFull(mouth[happyOpenMouth]);
    }
   
  }

  else if (state==4)  //ZowiPad mode
  {
    SCmd.readSerial();
    if (endmove==0)
    {
      move(moveId);
    }
  }


//-------------------------------------------
//Uncomment this for a animation demo (animation name = littleUuh)
// int time_anim = 200;

// for(int veces=0;veces<3;veces++){
//     for (int i=0;i<(sizeof(littleUuh)/sizeof(unsigned long int));i++)
//     {
//         ledmatrix.writeFull(littleUuh[i]);
//         delay(time_anim);
//     }
// }


//-------------------------------------------
//Uncomment this for a noise sensor test. Zowi's mouths will show the level of noise detected
/*noiseToLed(noiseLevel(PIN_NoiseSensor));
delay(100);
//Serial.println(analogRead(PIN_NoiseSensor));
*/

//-------------------------------------------
//Uncomment this for a noise sensor demo with funny faces and movements 
/*if (analogRead(PIN_NoiseSensor)>=740){
    //ledmatrix.writeFull(mouth[confused]);
    randomMouth(random(1,16));
    move(random(1,14));
    //zowi.updown(1,300,15);
    zowi.home();
    delay(500);
}
else {
    ledmatrix.writeFull(mouth[smile]);
    
}
*/

//-------------------------------------------
//Uncomment this for a obstacle detector.
/*
zowi.walk(1,1000,1);
surprised=0;

if (Distance(PIN_TriggerBat,PIN_EchoBat)<=15)
{ 
  if(surprised==0)
  {
    ledmatrix.writeFull(mouth[bigSurprise]);
    //zowi.updown(1,500,15);
    zowi.jump(5, 500);
    surprised=1;
  }
  ledmatrix.writeFull(mouth[confused]);
  zowi.walk(3,1000,-1); 
  zowi.turn(8,1000,1);
  zowi.home(); 
  ledmatrix.writeFull(mouth[happyOpenMouth]);
}
*/

//-------------------------------------------
//Uncomment this for a simple US test
/*
if (Distance(PIN_TriggerBat,PIN_EchoBat)<=15)
{ 
  if(surprised==0)
  {
    ledmatrix.writeFull(mouth[bigSurprise]);
    //zowi.updown(1,500,15);
    zowi.jump(5, 500);
    surprised=1;
  }
  ledmatrix.writeFull(mouth[confused]);
  zowi.walk(1,1000,-1);  
}
else 
{
  surprised=0;
  ledmatrix.writeFull(mouth[happyOpenMouth]);
  zowi.home();
}
*/

//-------------------------------------------
//Uncomment this for a Robopad++ test
/*char crec = 0;

  if(Serial.available() != 0) {
    crec = Serial.read();
    if(crec == 'U') {
      ledmatrix.writeFull(mouth[smile]);
      zowi.walk(1,T,1);
    } else if(crec == 'D') {
      ledmatrix.writeFull(mouth[sad]);
      zowi.walk(1,T,-1);
    } else if(crec == 'L') {
      ledmatrix.writeFull(mouth[confused]);
      zowi.turn(4,T,1);
    } else if(crec == 'R') {
      ledmatrix.writeFull(mouth[bigSurprise]);
      zowi.turn(4,T,-1);
    } else {zowi.home();}
    while(Serial.available() != 0) {
    crec = Serial.read();}
  
  }
  */

//-------------------------------------------
//Uncomment this for a ZowiPad Test
 /* SCmd.readSerial();
  
  if (endmove==0){
    move(moveId);
  }
*/

//-------------------------------------------
//Uncomment this for a movement test with the third button

/*if (digitalRead(PIN_ThirdButton)==1){
  moveId++;
}
if (moveId==14){
  moveId=0;
}
move(moveId);
*/

//-------------------------------------------
//Uncomment this for a constant movement
//zowi.walk(1,1000,1);

//-------------------------------------------
//Uncomment this for a constant Batery Level test with the serial monitor.
/*float bateryLevel=(map(analogRead(A7),0,1023,0,500));
bateryLevel=bateryLevel/100;
Serial.println(bateryLevel);
delay(1000);*/


}




////////////////////////////
// Functions              //
////////////////////////////


//Function executed when second button is pushed
void secondButtonPushed(){  
    if(state<4 && buttonPushed==0)
    {
      state++; 
           
    }  
    buttonPushed=1;
    ledmatrix.writeFull(mouth[state]);  
}

//Function executed when third button is pushed
void thirdButtonPushed(){ 
    if(state>1 && buttonPushed==0)
    {
      state--;
    }
    buttonPushed=1;
    ledmatrix.writeFull(mouth[state]);
}




//Function to receive LED commands
void receiveLED(){  

  sendAck();

  //Examples of receiveLED Bluetooth commands
  //L 00000000100001010010001100000000
  //L 00111111111111111111111111111111 (todos los LED encendidos)
  unsigned long int matrix;
  char *arg;
  char *endstr;
  arg=SCmd.next();
  //Serial.println (arg);
  if (arg != NULL) 
  {
      matrix=strtoul(arg,&endstr,2);    // Converts a char string to an integer 
      ledmatrix.writeFull(matrix);
  } 
}


//Function to receive movement commands
void receiveMovement(){

  sendAck();

  //Definition of Movement Bluetooth commands
  //M  MoveID  T   MoveSize  
  char *arg; 
  arg = SCmd.next(); 
  if (arg != NULL) {moveId=atoi(arg);}
  else 
  {
    ledmatrix.writeFull(mouth[xMouth]);
    delay(4000);
    ledmatrix.clearMatrix();
    moveId=0;
  }
  arg = SCmd.next(); 
  if (arg != NULL) {T=atoi(arg);}
  else 
  {
    ledmatrix.writeFull(mouth[xMouth]);
    delay(4000);
    ledmatrix.clearMatrix();
    T=1000;
  }
  arg = SCmd.next(); 
  if (arg != NULL) {moveSize=atoi(arg);}
  else 
  {
    moveSize =30;
  }
  /* Desmarcar para coger parametro direccion 
  arg = SCmd.next(); 
  if (arg != NULL) 
    { 
      if (*arg=='F'){dir=1;}
      if (*arg=='B'){dir=-1;}
    }
  else 
    {
      ledmatrix.writeFull(mouth[xMouth]);
      delay(4000);
      ledmatrix.clearMatrix();
    }
    */
    endmove=0;
}

//Function to receive stop commands. Stop is a reserved word.
void stopp(){

    sendAck();

    endmove=1;
    zowi.home();
}

//Function to execute the right movement according the movement command received.
void move(int moveId){
    switch (moveId) {
      case 1: //M 1 1000 
        zowi.walk(1,T,1);
        break;
      case 2: //M 2 1000 
        zowi.walk(1,T,-1);
        break;
      case 3: //M 3 1000 
        zowi.turn(1,T,1);
        break;
      case 4: //M 4 1000 
        zowi.turn(1,T,-1);
        break;
      case 5: //M 5 1000 30 
        zowi.updown(1,T,moveSize);
        break;
      case 6: //M 6 1000 30
        zowi.moonwalker(1,T,moveSize,1);
        break;
      case 7: //M 7 1000 30
        zowi.moonwalker(1,T,moveSize,-1);
        break;
      case 8: //M 8 1000 30
        zowi.swing(1,T,moveSize);
        break;
      case 9: //M 9 1000 30 
        zowi.crusaito(1,T,moveSize,1);
        break;
      case 10: //M 10 1000 30 
        zowi.crusaito(1,T,moveSize,-1);
        break;
      case 11: //M 11 1000 
        zowi.jump(1,T);
        break;
      case 12: //M 12 1000 30 
        zowi.flapping(1,T,moveSize,1);
        break;
      case 13: //M 13 1000 30
        zowi.flapping(1,T,moveSize,-1);
        break;
      case 14: //M 14 1000 20
        zowi.tiptoeSwing(1,T,moveSize);
        break;
      case 15: //M 15 500 
        zowi.bend(1,T,1);
        break;
      case 16: //M 16 500 
        zowi.bend(1,T,-1);
        break;
      case 17: //M 17 500 
        zowi.shakeLeg(4,T,1);
        break;
      case 18: //M 18 500 
        zowi.shakeLeg(4,T,1);
        break;
      case 19: //M 19 500 20
        zowi.jitter(1,T,moveSize);
        break;
      case 20: //M 20 500 15
        zowi.ascendingTurn(1,T,moveSize);
        break;
      default:
        break;// do something
      }
    }

//Function to receive buzzer commands
void recieveBuzzer(){
  
  sendAck();

  int frec; 
  int duration; 
  char *arg; 
  arg = SCmd.next(); 
  if (arg != NULL) 
    {
      frec=atoi(arg);    // Converts a char string to an integer   
    } 
  else 
    {
      ledmatrix.writeFull(mouth[xMouth]);
      delay(4000);
      ledmatrix.clearMatrix();
    } 
  arg = SCmd.next(); 
  if (arg != NULL) 
    { 
      duration=atoi(arg);    // Converts a char string to an integer  
    }
  else
    {
      ledmatrix.writeFull(mouth[xMouth]);
      delay(4000);
      ledmatrix.clearMatrix();
    } 

  ZowiTone(PIN_Buzzer, frec, duration, duration);   
  // tone(PIN_Buzzer,frec,duration);
  // delay(duration);
  // noTone(PIN_Buzzer);
}



//Ultrasound Sensor
long TP_init(int trigger_pin, int echo_pin)
   {
    digitalWrite(trigger_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger_pin, LOW);
    long microseconds = pulseIn(echo_pin ,HIGH,100000);
    return microseconds;
}

long Distance(int trigger_pin, int echo_pin)
  {
    long microseconds = TP_init(trigger_pin, echo_pin);
    long distance;
    distance = microseconds/29/2;
    if (distance == 0){
      distance = 999;
    }
    return distance;
}


void requestDistance()
{
    int distance=Distance(PIN_TriggerBat,PIN_EchoBat);
    Serial.print("&&");
    Serial.print("D ");
    Serial.print(distance);
    Serial.println("%%");
    Serial.flush();
}

void requestNoise()
{
    int microphone=analogRead(PIN_NoiseSensor);
    Serial.print("&&");
    Serial.print("N ");
    Serial.print(microphone);
    Serial.println("%%");
    Serial.flush();
}

void requestBattery()
{
       //The first read of the batery is often a wrong reading, so we read it two times. 
    double batteryLevel= battery.readBatPercent();
    batteryLevel= battery.readBatPercent();

    Serial.print("&&");
    Serial.print("B ");
    Serial.print(batteryLevel);
    Serial.println("%%");
    Serial.flush();
}

void requestProgramId()
{
    Serial.print("&&");
    Serial.print("I ");
    Serial.print(programID);
    Serial.println("%%");
    Serial.flush();
}

void sendAck()
{
    Serial.print("&&");
    Serial.print("A");
    Serial.println("%%");
    Serial.flush();
}


void ZowiTone(int speakerPin, float noteFrequency, long noteDuration, long silentDuration)
{    
  int x;
  // Convert the frequency to microseconds
  float microsecondsPerWave = 1000000/noteFrequency;
  // Calculate how many HIGH/LOW cycles there are per millisecond
  float millisecondsPerCycle = 1000/(microsecondsPerWave * 2);
  // Multiply noteDuration * number or cycles per millisecond
  float loopTime = noteDuration * millisecondsPerCycle;
  // Play the note for the calculated loopTime.
  for (x=0;x<loopTime;x++)   
  {   
              digitalWrite(speakerPin,HIGH); 
              delayMicroseconds(microsecondsPerWave); 
              digitalWrite(speakerPin,LOW); 
              delayMicroseconds(microsecondsPerWave); 
  }
  delay(silentDuration); 
}  




//Pruebas sonidoo
//---
void ZowiMelody(float melodyNotes[], int melodySize){

     for (int i=0;i<=melodySize;i=i+3) //por cada "fila"
     {
        ZowiTone(PIN_Buzzer, melodyNotes[i],melodyNotes[i+1],melodyNotes[i+2]);
     }

}

void conexion (){
          ZowiTone(PIN_Buzzer, note_E6,50,30);
          ZowiTone(PIN_Buzzer, note_E7,50,30);
          ZowiTone(PIN_Buzzer, note_A7,50,30);
}

void desconexion (){
          ZowiTone(PIN_Buzzer, note_E6,50,30);
          ZowiTone(PIN_Buzzer, note_A7,50,30);
          ZowiTone(PIN_Buzzer, note_E7,50,30);
}








//Constant feedback of the sensors. F 1 = Feedback ON  F 0 = Feedback 0
/*void sensorFeedback()
{
  char *arg; 
  arg = SCmd.next(); 
  if (arg != NULL) 
    {
      sensorFeedbackOn=atoi(arg);    // Converts a char string to an integer   
    } 
  else 
    {
      ledmatrix.writeFull(mouth[xMouth]);
      delay(4000);
      ledmatrix.clearMatrix();
    } 
}
*/
/*if (sensorFeedbackOn)
{ 
    //Constant feedback of the sensors. Put this code in the loop
    Serial.print("&&");
    Serial.print("D ");
    Serial.print(distance);
    Serial.println("%%");
    delay(10);
    Serial.print("&&");
    Serial.print("M ");
    Serial.print(microphone);
    Serial.println("%%");
    delay(10);
    Serial.print("&&");
    Serial.print("B ");
    Serial.print(bateryLevel);
    Serial.println("%%");
    delay(10);
}
*/



// void lineaLeds(int linea){
//     for (int i=1;i<7;i++){
//       ledmatrix.setLed(linea, i);
//     }
// }
                                                                             
// void lineaLedsOff(int linea){
//     for (int i=1;i<7;i++){
//       ledmatrix.unsetLed(linea, i);
//     }
// }

// int noiseLevel(int PinNoiseSensor){
//     int i = map(analogRead(PinNoiseSensor),400,800,1,6);
//     return i;
// }

// void noiseToLed(int noiseLevel){
//     switch (noiseLevel){
//       case 5: 
//       lineaLeds(1);
//       case 4: 
//       lineaLeds(2);   
//       case 3: 
//       lineaLeds(3);    
//       case 2: 
//       lineaLeds(4);    
//       case 1: 
//       lineaLeds(5);
//       break;
//       default: break;
//     }
//     switch (noiseLevel){
//       case 1: 
//       lineaLedsOff(4);
//       case 2: 
//       lineaLedsOff(3);
//       case 3: 
//       lineaLedsOff(2);
//       case 4: 
//       lineaLedsOff(1);
//       break;
//       default: break;
//     }
// }







