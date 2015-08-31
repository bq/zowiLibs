#ifndef Zowi_h
#define Zowi_h

#include <US.h>
#include <Zowi_mouths.h>
#include <BatReader.h>
#include <LedMatrix.h>

#include <Servo.h>
#include <Oscillator.h>
#include <EEPROM.h>



//-- Constants
#define FORWARD   1
#define BACKWARD -1
#define LEFT      1
#define RIGHT    -1
#define SMALL 5
#define MEDIUM 15
#define BIG 30


class Zowi
{
  public:
    Zowi();
    void init(int YL, int YR, int RL, int RR, bool load_calibration=0);
    void setTrims(int YL, int YR, int RL, int RR);
    void saveTrimsOnEEPROM();
    
    void moveServos(int time, int  servo_target[]);
    void oscillateServos(int A[4], int O[4], int T, double phase_diff[4], float cycle);
    
    void home();
    void homeold();
    void walk(float steps=4, int T=1800, int dir = FORWARD);
    void turn(float steps=1, int T=2000, int dir = LEFT);
    void updown(float steps=1, int T=1000, int h = 20);
    void moonwalker(float steps, int T, int h, int dir);
    void swing(float steps, int T, int h);
    void tiptoeSwing(float steps, int T, int h);
    void crusaito(float steps, int T, int h, int dir);
    void flapping(float steps, int T, int h, int dir);
    void jump(float steps=1, int T = 8000);
    void shakeLeg (int steps=2,int T = 500,int dir=RIGHT);
    void bend (int steps=1, int T=2000, int dir=LEFT);
    void jitter(float steps, int T, int h);
    void ascendingTurn(float steps, int T, int h);
    
    void printMouth(int mouthType);
    void clearMouth();

    int getDistance();
    double getBattery();
 
  private:
  
    Oscillator servo[4];
    LedMatrix mouth;
    BatReader battery;
    US usSensor;


    int servo_trim[4];
    int servo_position[4];
    unsigned long final_time;
    unsigned long partial_time;
    float increment[4];
    unsigned long int mouthType[];
    //**ANIMATIONS***
    unsigned long int adivinawi[];
    unsigned long int littleUuh[];
    unsigned long int wave[];
    unsigned long int zzzZ[];
    unsigned long int dreamMouth[];
    unsigned long int spiral[];  

    void _execute(int A[4], int O[4], int T, double phase_diff[4], float steps);
};

#endif


