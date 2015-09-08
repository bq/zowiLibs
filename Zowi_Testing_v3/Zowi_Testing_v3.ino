
//----------------------------------------------------------------
//-- Zowi: Testing the complete pack
//-- (c) BQ. Released under a GPL licencse
//-- August 2015
//-- Authors:  Javier Isabel:  javier.isabel@bq.com
//--           Juan Gonzalez (obijuan): juan.gonzalez@bq.com
//--           Jose Alberca:   jose.alberca@bq.com
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

//---Zowi Sounds
#include <Zowi_sounds.h>


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

          #define PIN_YL 3 //servo[0]
          #define PIN_YR 2 //servo[1]
          #define PIN_RL 5 //servo[2]
          #define PIN_RR 4 //servo[3]
          
          
          

//-- Second step:  Determine the Servo Trim values
/*
//EVT3-1
#define TRIM_RR   -2
#define TRIM_RL   -9
#define TRIM_YR   4
#define TRIM_YL  0
//EVT2-1
#define TRIM_RR   -20
#define TRIM_RL   -18
#define TRIM_YR   -0
#define TRIM_YL  3
*/

//EVT3-2 *
#define TRIM_RR   8
#define TRIM_RL   -22
#define TRIM_YR   4
#define TRIM_YL  6




//bqBAT
long TP_init(int trigger_pin, int echo_pin);
long Distance(int trigger_pin, int echo_pin);
#define PIN_TriggerBat 8
#define PIN_EchoBat 9

//Noise Sensor
#define PIN_NoiseSensor A6

//Buzzer
#define PIN_Buzzer0 10

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
volatile int state=0; //State of zowi state machine
volatile int buttonPushed=0; //Variable to remember when a button has been pushed
volatile int buttonAPushed=0; //Variable to remember when A button has been pushed
volatile int buttonBPushed=0; //Variable to remember when B button has been pushed
unsigned long previousMillis=0;
int sensorFeedbackOn=0; //Variable to activate the sensor comunication from Zowi to the App

int randomDance=0;
int randomSteps=0;


////////////////////////////
// Setup                  //
////////////////////////////

void setup() {

  //Serial communication initialization
  Serial.begin(115200);  
  
  //PinMode of the components
  pinMode(PIN_EchoBat, INPUT );
  pinMode(PIN_TriggerBat, OUTPUT );
  pinMode(PIN_Buzzer0,OUTPUT);
  pinMode(PIN_SecondButton,INPUT);
  pinMode(PIN_ThirdButton,INPUT);
  
  //Set the servo pins
  zowi.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR,true);


  //Uncomment this to set the servo trims manually and save on EEPROM 
  //zowi.setTrims(TRIM_YL, TRIM_YR, TRIM_RL, TRIM_RR);
  //zowi.saveTrimsOnEEPROM(); //Uncomment this only for one upload when you finaly set the trims.


  //RandomSeed
  randomSeed(analogRead(PIN_NoiseSensor));

  //Interrumptions
  enableInterrupt(PIN_SecondButton, secondButtonPushed, RISING);
  enableInterrupt(PIN_ThirdButton, thirdButtonPushed, RISING);

  // Setup callbacks for SerialCommand commands 
  SCmd.addCommand("S", stopp);            //  sendAck();
  SCmd.addCommand("L", receiveLED);       //  sendAck();
  SCmd.addCommand("T", recieveBuzzer);    //  sendAck();
  SCmd.addCommand("M", receiveMovement);  //  sendAck();
  SCmd.addCommand("R", receiveName);      //  sendAck();
  SCmd.addCommand("E", requestName);
  SCmd.addCommand("D", requestDistance);
  SCmd.addCommand("N", requestNoise);
  SCmd.addCommand("B", requestBattery);
  SCmd.addCommand("I", requestProgramId);


  SCmd.addDefaultHandler(stopp);


  //Checking battery
  //ZowiLowBatteryAlarm();

  //-- Zowi wake up!
  //A little moment of initial surprise
  zowi.home();

  //ledmatrix.writeFull(mouthType[bigSurprise]);
  S_connection(); 


  //animation uuuh
  int time_anim = 150;
  for (int i=0;i<(sizeof(littleUuh)/sizeof(unsigned long int));i++)
  {
      ledmatrix.writeFull(littleUuh[i]);
      delay(time_anim);
  }

  for (int i=0;i<(sizeof(littleUuh)/sizeof(unsigned long int));i++)
  {
      ledmatrix.writeFull(littleUuh[i]);
      delay(time_anim);
  }
  

  //Smile for a happy Zowi
  ledmatrix.writeFull(mouthType[smile]);
  S_happy();

  delay(200);
  zowi.jump(1,700);
  delay(200);
  zowi.shakeLeg(1,T,1);
  
  delay(200);
  ledmatrix.writeFull(mouthType[smallSurprise]);
  zowi.swing(2,800,20);  

    //Zowi's resting position
  zowi.home();
  ledmatrix.writeFull(mouthType[happyOpenMouth]);


  previousMillis = millis();
  

}



////////////////////////////
// Loop                   //
////////////////////////////
void loop() {

  //First attemp to initial software
  if (buttonPushed)
  {  

    
    zowi.home();

    delay(100); //Wait for all buttons 
    S_buttonPushed();
    delay(200); //Wait for all buttons 

    if(buttonAPushed==1 && buttonBPushed==0){      state=1; S_mode1();}
    else if(buttonAPushed==0 && buttonBPushed==1){ state=2; S_mode2();}
    else if(buttonAPushed==1 && buttonBPushed==1){ state=3; S_mode3();} //else

    ledmatrix.writeFull(mouthType[state]);

    delay(800); //Wait to show the state number 
    ledmatrix.writeFull(mouthType[happyOpenMouth]);

    buttonPushed=0;
    buttonAPushed=0;
    buttonBPushed=0;

  }


  if  (Serial.available()>0 && state!=4)
  {
    state=4;
    //ledmatrix.writeFull(mouthType[state]);
    //zowi.home();
    //delay(500); //Wait to show the state number 
    ledmatrix.writeFull(mouthType[happyOpenMouth]);
  }


  
  //----------------------------------

  switch (state) {


      //MODE 0 - Zowi Wait
      case 0:
        
      
        if (millis()-previousMillis>=10000)
        {
          //Zowi se duerme!!
          for(int i=0; i<4;i++){

            ZowiDreaming();
            if(buttonPushed){break;}  
          }

          if(!buttonPushed){
            ledmatrix.writeFull(mouthType[lineMouth]);
            S_confused();
            previousMillis=millis();
          }
          
          zowi.home();
          if(!buttonPushed){ledmatrix.writeFull(mouthType[happyOpenMouth]);} 
          
        }
        break;
      

      //MODE 1 - DANCE STATE!!
      case 1:
        
        randomDance=random(5,21); //5,20
        if((randomDance>14)&&(randomDance<19)){
              randomSteps=1;
              T=1600;
        }
        else{
              randomSteps=random(3,6);  //3,5
              T=1000;
        }
        
        ledmatrix.writeFull(mouthType[random(10,21)]);

        for (int i=0;i<randomSteps;i++)
        {
          move(randomDance);
          if(buttonPushed){break;}
        }
        break;


      //MODE 2 - Obstacle detector mode
      case 2:

        zowi.walk(1,1000,1);

        if (Distance(PIN_TriggerBat,PIN_EchoBat)<=15)  //15cm
        { 
          ledmatrix.writeFull(mouthType[bigSurprise]);
          S_surprise();
          zowi.jump(5, 500);

          if(!buttonPushed)
          {
            ledmatrix.writeFull(mouthType[confused]);
            for(int i=0;i<3;i++)
            {
              if(buttonPushed){break;}
              zowi.walk(1,1000,-1);
            } 
            for (int i=0;i<4;i++){
              if(buttonPushed){break;}
              zowi.turn(1,1000,1);
            }
          }

          if(!buttonPushed){ledmatrix.writeFull(mouthType[happyOpenMouth]);}
        
        }
        break;


      //MODE 3 - Noise sensor mode  
      case 3:
        if (analogRead(PIN_NoiseSensor)>=740)
        {
          
          delay(50);  //Wait for the possible 'lag' of the button interruptions. 
                      //Sometimes, the noise sensor detect the button before the interruption takes efect 
          
          if(!buttonPushed){

            ledmatrix.writeFull(mouthType[bigSurprise]);
            S_OhOoh();

            if(buttonPushed){break;}
            ledmatrix.writeFull(mouthType[random(10,21)]);
            move(random(1,20));
            zowi.home();
            delay(50); //Wait for possible noise of the servos while get home
          }
          
          if(!buttonPushed){ledmatrix.writeFull(mouthType[happyOpenMouth]);}
        }
        break;
        

      //MODE 3 - ZowiPad control  
      case 4:

        if(zowi.getStatus()==false){

          SCmd.readSerial();
          if (endmove==0) 
          {
            move(moveId);
          }

        }
        
        break;
        

      default:
          state=4;
          break;
        
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
//Uncomment this for example melody code:
//    int sssize = (sizeof(connectionNotes)/sizeof(float));
//    ZowiMelody(connectionNotes, sssize);



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
    // if(state<4 && buttonPushed==0)
    // {
    //   state++; 
           
    // }  
    buttonPushed=1;
    buttonAPushed=1;
    ledmatrix.writeFull(mouthType[smallSurprise]);  
}

//Function executed when third button is pushed
void thirdButtonPushed(){ 
    // if(state>1 && buttonPushed==0)
    // {
    //   state--;
    // }
    buttonPushed=1;
    buttonBPushed=1;
    ledmatrix.writeFull(mouthType[smallSurprise]);
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



//Function to receive stop commands. Stop is a reserved word.
void stopp(){

    sendAck();

    endmove=1;
    zowi.home();
}

//Function to receive LED commands
void receiveLED(){  

  //sendAck();
  stopp();


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
      matrix=strtoul(arg,&endstr,2);    // Converts a char string to unsigned long integer
      ledmatrix.writeFull(matrix);
  } 
}

//Function to receive buzzer commands
void recieveBuzzer(){
  
  //sendAck();
  stopp();

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
      ledmatrix.writeFull(mouthType[xMouth]);
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
      ledmatrix.writeFull(mouthType[xMouth]);
      delay(4000);
      ledmatrix.clearMatrix();
    } 

  ZowiTone(PIN_Buzzer0, frec, duration, 0);   
  // tone(PIN_Buzzer0,frec,duration);
  // delay(duration);
  // noTone(PIN_Buzzer0);
}


//Function to receive movement commands
void receiveMovement(){

  sendAck();
  //stopp();

  //Definition of Movement Bluetooth commands
  //M  MoveID  T   MoveSize  
  char *arg; 
  arg = SCmd.next(); 
  if (arg != NULL) {moveId=atoi(arg);}
  else 
  {
    ledmatrix.writeFull(mouthType[xMouth]);
    delay(4000);
    ledmatrix.clearMatrix();
    moveId=0;
  }
  arg = SCmd.next(); 
  if (arg != NULL) {T=atoi(arg);}
  else 
  {
    ledmatrix.writeFull(mouthType[xMouth]);
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
        zowi.shakeLeg(1,T,1);
        break;
      case 18: //M 18 500 
        zowi.shakeLeg(1,T,-1);
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

    //endmove=1; 
}


void receiveName(){

  stopp();

  char newZowiName[11] = "";  //Variable to store data read from Serial.
  int eeAddress = 5;          //Location we want the data to be in EEPROM.
  char *arg; 
  arg = SCmd.next(); 
  
  if (arg != NULL) {

    //Complete newZowiName char string
    int k = 0;
    while((*arg) && (k<11)){ 
        newZowiName[k]=*arg++;
        k++;
    }
    
    EEPROM.put(eeAddress, newZowiName); 

  }
  else 
  {
    ledmatrix.writeFull(mouthType[xMouth]);
    delay(4000);
    ledmatrix.clearMatrix();
  } 
}


void requestName(){


  char actualZowiName[11]= "";  //Variable to store data read from EEPROM.
  int eeAddress = 5;            //EEPROM address to start reading from
  

  //Get the float data from the EEPROM at position 'eeAddress'
  EEPROM.get(eeAddress, actualZowiName);


  Serial.print("&&");
  Serial.print(actualZowiName);
  Serial.println("%%");
  Serial.flush();

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


void ZowiDreaming(){

  ledmatrix.writeFull(dreamMouth[0]);
  for (int i=100; i<200; i=i*1.04) {
        ZowiTone(PIN_Buzzer,i,10,10);
  }
  
  ledmatrix.writeFull(dreamMouth[1]);
  for (int i=200; i<300; i=i*1.04) {
        ZowiTone(PIN_Buzzer,i,10,10);
  }

  ledmatrix.writeFull(dreamMouth[2]);
  for (int i=300; i<500; i=i*1.04) {
        ZowiTone(PIN_Buzzer,i,10,10);
  }

  delay(500);
  
  ledmatrix.writeFull(dreamMouth[1]);
  for (int i=400; i>250; i=i/1.04) {
        ZowiTone(PIN_Buzzer,i,10,0);
  }


  ledmatrix.writeFull(dreamMouth[0]);
  for (int i=250; i>100; i=i/1.04) {
        ZowiTone(PIN_Buzzer,i,10,0);
  }

  delay(500);
}



void ZowiLowBatteryAlarm(){

  //The first read of the batery is often a wrong reading, so we read it two times. 
  double batteryLevel= battery.readBatPercent();
  batteryLevel= battery.readBatPercent();

  if(batteryLevel<80){
      
      while(!buttonPushed){

          ledmatrix.writeFull(mouthType[thunder]);
          for (int i=880; i<2000; i=i*1.04) {  //A5 = 880
            ZowiTone(PIN_Buzzer,i,8,3);
          }
          delay(30);
          for (int i=2000; i>880; i=i/1.02) {  //A5 = 880
            ZowiTone(PIN_Buzzer,i,8,3);
          }

          ledmatrix.clearMatrix();
          delay(500);
      }
       
  }

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






