

//----------------------------------------------------------------
//-- Zowi Game: Electronic Dice
//-- (c) BQ. Released under a GPL licencse
//-- September 2015
//-- Authors:  Anita de Prado: ana.deprado@bq.com
//-----------------------------------------------------------------
//-- Experiment with all the features that Zowi have!
//-----------------------------------------------------------------

#include <Servo.h>
#include <Oscillator.h>
#include <EEPROM.h>
#include <ZowiSerialCommand.h>

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

          #define PIN_YL 2 //servo[0]
          #define PIN_YR 3 //servo[1]
          #define PIN_RL 4 //servo[2]
          #define PIN_RR 5 //servo[3]


//---Zowi US sensor
long TP_init(int trigger_pin, int echo_pin);
long Distance(int trigger_pin, int echo_pin);
#define PIN_TriggerBat 8
#define PIN_EchoBat 9

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

int programID=2; //Each program will have a ID in order to 

char name_fac='$'; //Factory name
char name_fir='#'; //First name

int trim_YL = 0;  //servo[0]
int trim_YR = 0;  //servo[1]
int trim_RL = 0;  //servo[2]
int trim_RR = 0;  //servo[3]

int T=1000; //Initial duration of movement
int moveId=0; //Number of movement
int endmove=1; // 1= stop 0= moving
int moveSize=15; //Many movements accept a moveSize parameter asociated with the height of that movement

volatile int state=0; //State of zowi state machine
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
  pinMode(PIN_EchoBat,INPUT);
  pinMode(PIN_TriggerBat,OUTPUT);
  pinMode(PIN_Buzzer0,OUTPUT);
  pinMode(PIN_SecondButton,INPUT);
  pinMode(PIN_ThirdButton,INPUT);
  
  //Set the servo pins
  zowi.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR,true);

  //RandomSeed
  randomSeed(analogRead(PIN_NoiseSensor));

  //Setup callbacks for SerialCommand commands 
  SCmd.addCommand("S", stopp);            //  sendAck();
  SCmd.addCommand("L", receiveLED);       //  sendAck();
  SCmd.addCommand("T", recieveBuzzer);    //  sendAck();
  SCmd.addCommand("M", receiveMovement);  //  sendAck();
  SCmd.addCommand("C", receiveTrims);     //  sendAck();
  SCmd.addCommand("G", receiveServo);     //  sendAck();
  SCmd.addCommand("R", receiveName);      //  sendAck();
  SCmd.addCommand("E", requestName);
  SCmd.addCommand("D", requestDistance);
  SCmd.addCommand("N", requestNoise);
  SCmd.addCommand("B", requestBattery);
  SCmd.addCommand("I", requestProgramId);
  SCmd.addDefaultHandler(stopp);


  S_connection();
  zowi.home();
 

  //--Send Zowi name, programID & battery level by bluetooth.
  requestName();
  requestProgramId();
  requestBattery();


  //Checking battery
  ZowiLowBatteryAlarm();


 //--
 // Zowi wake up! A little moment of initial surprise
 //--
 //Animation uuuh
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

  ledmatrix.writeFull(mouthType[happyOpenMouth]);


  previousMillis = millis();
  

}



////////////////////////////
// Loop                   //
////////////////////////////
void loop() {

  if(Serial.available()>0 && state!=4)
  {
    state=4;
    ledmatrix.writeFull(mouthType[happyOpenMouth]);
  }

  
  //----------------------------------

  switch (state) {

      //MODE 0 - Zowi Wait
      case 0:
      
      
        if (millis()-previousMillis>=30000)
        {
          previousMillis = 0;
          
          //Zowi se duerme!!
          for(int i=0; i<4;i++){
            ZowiDreaming(); 
          }

          ledmatrix.writeFull(mouthType[lineMouth]);
          S_confused();
          previousMillis=millis();
          
          ledmatrix.writeFull(mouthType[happyOpenMouth]);
          
        }

        if (analogRead(PIN_NoiseSensor)>=740)
        {

          ledmatrix.writeFull(mouthType[smallSurprise]);
          
          S_buttonPushed();
          zowi.swing(1,500,10);

          zowi.home(); 

          int randomNum = random(1,7); //1-6
          ledmatrix.writeFull(mouthType[randomNum]);


          if(randomNum==1){         //1
            S_love();
          }else if(randomNum==6){   //6
            S_superHappy();    
          }else{                    //2,3,4,5
            Hello_short();
          }

          
          

          delay(2000); 


          ledmatrix.writeFull(mouthType[happyOpenMouth]);

          previousMillis = millis();

        }

        break;
      

      //MODE 4 - ZowiPad control  
      case 4:

        SCmd.readSerial();
        if (endmove==0) 
        {
          move(moveId);
        }

        break;
        

      default:
          state=4;
          break;
        
  }

}




////////////////////////////
// Functions              //
////////////////////////////

//Ultrasound Sensor
long TP_init(int trigger_pin, int echo_pin)
{
    digitalWrite(trigger_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger_pin, LOW);
    long microseconds = pulseIn(echo_pin,HIGH,100000);
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

  stopp(); //stop and sendAck

  //Examples of receiveLED Bluetooth commands
  //L 00000000100001010010001100000000
  //L 00111111111111111111111111111111 (todos los LED encendidos)
  unsigned long int matrix;
  char *arg;
  char *endstr;
  arg=SCmd.next();
  //Serial.println (arg);
  if (arg != NULL) {
      matrix=strtoul(arg,&endstr,2);    // Converts a char string to unsigned long integer
      ledmatrix.writeFull(matrix);
  }else {
    ledmatrix.writeFull(mouthType[xMouth]);
    delay(2000);
    ledmatrix.clearMatrix();
  }
}

//Function to receive buzzer commands
void recieveBuzzer(){
  
  stopp(); //stop and sendAck

  bool error = false; 
  int frec;
  int duration; 
  char *arg; 
  
  arg = SCmd.next(); 
  if (arg != NULL) { frec=atoi(arg); }    // Converts a char string to an integer   
  else {error=true;}
  
  arg = SCmd.next(); 
  if (arg != NULL) { duration=atoi(arg); } // Converts a char string to an integer  
  else {error=true;}

  if(error==true){
      ledmatrix.writeFull(mouthType[xMouth]);
      delay(2000);
      ledmatrix.clearMatrix();

  }else{ 

    ZowiTone(frec, duration, 0);   
  }
}

//Function to receive TRims commands
void receiveTrims(){  

  sendAck();

  //Definition of Servo Bluetooth command
  //C trim_YL trim_YR trim_RL trim_RR
  //Examples of receiveTrims Bluetooth commands
  //C 20 0 -8 3
  bool error = false;
  char *arg;
  arg=SCmd.next();
  if (arg != NULL) { trim_YL=atoi(arg); }    // Converts a char string to an integer   
  else {error=true;}

  arg = SCmd.next(); 
  if (arg != NULL) { trim_YR=atoi(arg); }    // Converts a char string to an integer  
  else {error=true;}

  arg = SCmd.next(); 
  if (arg != NULL) { trim_RL=atoi(arg); }    // Converts a char string to an integer  
  else {error=true;}

  arg = SCmd.next(); 
  if (arg != NULL) { trim_RR=atoi(arg); }    // Converts a char string to an integer  
  else {error=true;}

  
  if(error==true){

      ledmatrix.writeFull(mouthType[xMouth]);
      delay(2000);
      ledmatrix.clearMatrix();

  }else{ //Save it on EEPROM
    zowi.setTrims(trim_YL, trim_YR, trim_RL, trim_RR);
    zowi.saveTrimsOnEEPROM(); //Uncomment this only for one upload when you finaly set the trims.
  } 
}

//Function to receive Servo commands
void receiveServo(){  

  sendAck();

  //Definition of Servo Bluetooth command
  //G  servo_YL servo_YR servo_RL servo_RR 
  //Example of receiveServo Bluetooth commands
  //G 90 85 96 78 
  bool error = false;
  char *arg;
  int servo_YL,servo_YR,servo_RL,servo_RR;

   arg=SCmd.next();
  if (arg != NULL) { servo_YL=atoi(arg); }    // Converts a char string to an integer   
  else {error=true;}

  arg = SCmd.next(); 
  if (arg != NULL) { servo_YR=atoi(arg); }    // Converts a char string to an integer  
  else {error=true;}

  arg = SCmd.next(); 
  if (arg != NULL) { servo_RL=atoi(arg); }    // Converts a char string to an integer  
  else {error=true;}

  arg = SCmd.next(); 
  if (arg != NULL) { servo_RR=atoi(arg); }    // Converts a char string to an integer  
  else {error=true;}

  
  if(error==true){

      ledmatrix.writeFull(mouthType[xMouth]);
      delay(2000);
      ledmatrix.clearMatrix();

  }else{ //Update Servo:

    int servoPos[4]={servo_YL, servo_YR, servo_RL, servo_RR}; 
    zowi.moveServos(200, servoPos);   //Move 200ms
    
  } 
}

//Function to receive movement commands
void receiveMovement(){

  sendAck();

  //Definition of Movement Bluetooth commands
  //M  MoveID  T   MoveSize  
  bool error = false;
  char *arg; 
  arg = SCmd.next(); 
  if (arg != NULL) {moveId=atoi(arg);}
  else 
  {
    ledmatrix.writeFull(mouthType[xMouth]);
    delay(2000);
    ledmatrix.clearMatrix();
    moveId=0;
  }
  arg = SCmd.next(); 
  if (arg != NULL) {T=atoi(arg);}
  else 
  {
    ledmatrix.writeFull(mouthType[xMouth]);
    delay(2000);
    ledmatrix.clearMatrix();
    T=1000;
  }
  arg = SCmd.next(); 
  if (arg != NULL) {moveSize=atoi(arg);}
  else 
  {
    moveSize =30;
  }
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
        break;
    }
}


void receiveName(){

  stopp(); //stop and sendAck

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
    delay(2000);
    ledmatrix.clearMatrix();
  } 
}


void requestName(){

   char actualZowiName[11]= "";  //Variable to store data read from EEPROM.
   int eeAddress = 5;            //EEPROM address to start reading from
  

   //Get the float data from the EEPROM at position 'eeAddress'
   EEPROM.get(eeAddress, actualZowiName);


  Serial.print("&&");
  Serial.print("E ");
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







//-- Functions with animatics
//--------------------------------------------------------

void ZowiLowBatteryAlarm(){

  //The first read of the batery is often a wrong reading, so we read it two times. 
  double batteryLevel= battery.readBatPercent();
  batteryLevel= battery.readBatPercent();

  if(batteryLevel<40){
      
      while(true){

          ledmatrix.writeFull(mouthType[thunder]);
          ZowiBendTones (880, 2000, 1.04, 8, 3);  //A5 = 880
          
          delay(30);

          ZowiBendTones (2000, 880, 1.02, 8, 3);  //A5 = 880
          ledmatrix.clearMatrix();
          delay(500);
      }
       
  }

}

void ZowiDreaming(){

  ledmatrix.writeFull(dreamMouth[0]);
  ZowiBendTones (100, 200, 1.04, 10, 10);

  ledmatrix.writeFull(dreamMouth[1]);
  ZowiBendTones (200, 300, 1.04, 10, 10);  
  
  ledmatrix.writeFull(dreamMouth[2]);
  ZowiBendTones (300, 500, 1.04, 10, 10);   

  delay(500);

  ledmatrix.writeFull(dreamMouth[1]);
  ZowiBendTones (400, 250, 1.04, 10, 0); 
  
  ledmatrix.writeFull(dreamMouth[0]);
  ZowiBendTones (250, 100, 1.04, 10, 0); 
  
  delay(500);
}






