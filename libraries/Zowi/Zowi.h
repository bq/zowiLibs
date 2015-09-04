#ifndef Zowi_h
#define Zowi_h

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
    void init(int YL, int YR, int RL, int RR, bool load_calibration=0);
    void setTrims(int YL, int YR, int RL, int RR);
    void saveTrimsOnEEPROM();
    
    void moveServos(int time, int  servo_target[]);
    void oscillateServos(int A[4], int O[4], int T, double phase_diff[4], float cycle);
    
    void home();
    void homeold();

    void jump(float steps=1, int T = 2000);

    void walk(float steps=4, int T=1000, int dir = FORWARD);
    void turn(float steps=4, int T=2000, int dir = LEFT);
    void bend (int steps=1, int T=1400, int dir=LEFT);
    void shakeLeg (int steps=1, int T = 2000, int dir=RIGHT);

    void updown(float steps=1, int T=1000, int h = 20);
    void swing(float steps=1, int T=1000, int h=20);
    void tiptoeSwing(float steps=1, int T=900, int h=20);
    void jitter(float steps=1, int T=500, int h=20);
    void ascendingTurn(float steps=1, int T=900, int h=20);

    void moonwalker(float steps=1, int T=900, int h=20, int dir=LEFT);
    void crusaito(float steps=1, int T=900, int h=20, int dir=FORWARD);
    void flapping(float steps=1, int T=1000, int h=20, int dir=FORWARD);

    bool getStatus();


    
 
  private:
  
    bool zowiBusy=false;            //control variable

    Oscillator servo[4];
    int servo_trim[4];
    int servo_position[4];
    unsigned long final_time;
    unsigned long partial_time;
    float increment[4];

    
    void _execute(int A[4], int O[4], int T, double phase_diff[4], float steps);
};

#endif


