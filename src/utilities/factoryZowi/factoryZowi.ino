//----------------------------------------------------------------
//-- helloZowi.ino
//-- (c) BQ. Released under a GPL licencse
//-- September 2015
//-- Author:  Anita de Prado: ana.deprado@bq.com
//--          Jose Alberca: jose.alberca@bq.com
//-----------------------------------------------------------------

#define PIN_TriggerBat 8
#define PIN_EchoBat 9

#define heart_code   0b00010010101101100001010010001100
#define xMouth_code  0b00100001010010001100010010100001
#define okMouth_code 0b00000001000010010100001000000000

//---Zowi Led Array Mouth
#include <LedMatrix.h>

LedMatrix ledmatrix(11, 13, 12);

//Zowi
#include <Oscillator.h>
#include <Servo.h>
#include <EEPROM.h>
#include <Zowi.h>

Zowi zowi;

//Zowi factory name
char fac_name='$';
int fac_eeAddress = 5;


#define PIN_YL 2 //servo[0]
#define PIN_YR 3 //servo[1]
#define PIN_RL 4 //servo[2]
#define PIN_RR 5 //servo[3]

#define TRIM_RR 0
#define TRIM_RL 0
#define TRIM_YR 0
#define TRIM_YL 0


int checkPosition[4]={70,110,60,120}; //!!!!!!!!!
int homePosition[4]={90,90,90,90};

//Buttons
#define PIN_SecondButton 6
#define PIN_ThirdButton 7

void setup() {

    pinMode(PIN_EchoBat, INPUT );
    pinMode(PIN_TriggerBat, OUTPUT );
    pinMode(PIN_SecondButton,INPUT);
    pinMode(PIN_ThirdButton,INPUT);

    zowi.init(PIN_YL, PIN_YR, PIN_RL, PIN_RR,false);
    zowi.setTrims(TRIM_YL, TRIM_YR, TRIM_RL, TRIM_RR);


    char n = EEPROM.read(fac_eeAddress);
    if(n!=fac_name){ //Only if Zowi name is not the factory name

        EEPROM.write(fac_eeAddress, fac_name); //Setting factory name on EEPROM

    }
    

    while((digitalRead(PIN_SecondButton)==1)||(digitalRead(PIN_ThirdButton)==1))
    {
      ledmatrix.writeFull(xMouth_code);
      delay(100); //To avoid to have overcharged the micro writing the led matrix
    }

    while((digitalRead(PIN_SecondButton)==0)||(digitalRead(PIN_ThirdButton)==0))
    {
  	  if (Distance(PIN_TriggerBat,PIN_EchoBat)<=15)  //15cm
      { 
        ledmatrix.writeFull(heart_code);
      }
      else
      {
        ledmatrix.setEntireMatrix();
      }

      delay(100);
    }

    zowi.moveServos(500, checkPosition);

    while((digitalRead(PIN_ThirdButton)==0))
    {
     delay(100);
    }

    ledmatrix.writeFull(okMouth_code);
    zowi.moveServos(500, homePosition);

}




void loop() {
  

}


//--Functions
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
