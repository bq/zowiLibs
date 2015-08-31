#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include <pins_arduino.h>
#endif


#include "Zowi.h"
#include <Oscillator.h>
#include <LedMatrix.h>

Zowi::Zowi(){
   unsigned long int dummy []= {zero_code,one_code,two_code,three_code,four_code,five_code,six_code,seven_code,eight_code, nine_code,smile_code,happyOpenMouth_code,happyClosedMouth_code,heart_code,bigSurprise_code,smallSurprise_code,tongueOut_code, vamp1_code,vamp2_code,lineMouth_code,confused_code,diagonalMouth_code,sad_code,sadOpenMouth_code,sadOpenMouth_code, okMouth_code, xMouth_code,interrogation_code,thunder_code,culito_code};
   for (int i = 0; i< (sizeof(dummy)/sizeof(*dummy)); i++){
      Zowi::mouthType[i] = dummy[i];
   }

   Zowi::adivinawi[0]= 0b00100001000000000000000000100001;
   Zowi::adivinawi[1]= 0b00010010100001000000100001010010;
   Zowi::adivinawi[2]= 0b00001100010010100001010010001100;
   Zowi::adivinawi[3]= 0b00000000001100010010001100000000;
   Zowi::adivinawi[4]= 0b00000000000000001100000000000000;
   Zowi::adivinawi[5]= 0b00000000000000000000000000000000;

   Zowi::littleUuh[0]= 0b00000000000000001100001100000000;
   Zowi::littleUuh[1]= 0b00000000000000000110000110000000;
   Zowi::littleUuh[2]= 0b00000000000000000011000011000000;
   Zowi::littleUuh[3]= 0b00000000001100010010001100000000;
   Zowi::littleUuh[4]= 0b00000000000000000110000110000000;
   Zowi::littleUuh[5]= 0b00000000000000001100001100000000;
   Zowi::littleUuh[6]= 0b00000000000000011000011000000000;
   Zowi::littleUuh[7]= 0b00000000000000110000110000000000;
   Zowi::littleUuh[8]= 0b00000000000000011000011000000000;

   Zowi::wave[0]= 0b00001100010010100001000000000000;
   Zowi::wave[1]= 0b00000110001001010000100000000000;
   Zowi::wave[2]= 0b00000011000100001000010000100000;
   Zowi::wave[3]= 0b00000001000010000100001000110000;
   Zowi::wave[4]= 0b00000000000001000010100100011000;
   Zowi::wave[5]= 0b00000000000000100001010010001100;
   Zowi::wave[6]= 0b00000000100000010000001001000110;
   Zowi::wave[7]= 0b00100000010000001000000100000011;
   Zowi::wave[8]= 0b00110000001000000100000010000001;
   Zowi::wave[9]= 0b00011000100100000010000001000000;

   Zowi::zzzZ[0]= 0b00011111000010000100001000011111;
   Zowi::zzzZ[1]= 0b00000000001111000010000100001111;
   Zowi::zzzZ[2]= 0b00000000000000000111000010000111;
   Zowi::zzzZ[3]= 0b00000000000000000000000011000011;
    
   Zowi::dreamMouth[0]= 0b00000000000000000000110000110000;
   Zowi::dreamMouth[1]= 0b00000000000000010000101000010000;
   Zowi::dreamMouth[2]= 0b00000000011000100100100100011000;
   Zowi::dreamMouth[3]= 0b00000000000000010000101000010000;

   Zowi::spiral[0]= 0b00111110100000101110100010111110;
   Zowi::spiral[1]= 0b00101110101010101010100010111110;
   Zowi::spiral[2]= 0b00111110100010111010000010111110;
   Zowi::spiral[3]= 0b00111110100010101010101010111010;

}


void Zowi::init(int YL, int YR, int RL, int RR, bool load_calibration) {
  servo[0].attach(YL);
  servo[1].attach(YR);
  servo[2].attach(RL);
  servo[3].attach(RR);

  if (load_calibration) {
    for (int i = 0; i < 4; i++) {
      int servo_trim = EEPROM.read(i);
      if (servo_trim > 128) servo_trim -= 256;
      servo[i].SetTrim(servo_trim);
    }
  }
  
  for (int i = 0; i < 4; i++) servo_position[i] = 90;
}

void Zowi::setTrims(int YL, int YR, int RL, int RR) {
  servo[0].SetTrim(YL);
  servo[1].SetTrim(YR);
  servo[2].SetTrim(RL);
  servo[3].SetTrim(RR);
}

void Zowi::saveTrimsOnEEPROM() {
  for (int i = 0; i < 4; i++) EEPROM.write(i, servo[i].getTrim());
}

void Zowi::moveServos(int time, int  servo_target[]) {
  if(time>10){
    for (int i = 0; i < 4; i++)	increment[i] = ((servo_target[i]) - servo_position[i]) / (time / 10.0);
    final_time =  millis() + time;

    for (int iteration = 1; millis() < final_time; iteration++) {
      partial_time = millis() + 10;
      for (int i = 0; i < 4; i++) servo[i].SetPosition(servo_position[i] + (iteration * increment[i]));
      while (millis() < partial_time); //pause
    }
  }
  else{
    for (int i = 0; i < 4; i++) servo[i].SetPosition(servo_target[i]);
  }
  for (int i = 0; i < 4; i++) servo_position[i] = servo_target[i];
}

void Zowi::oscillateServos(int A[4], int O[4], int T, double phase_diff[4], float cycle=1){
  for (int i=0; i<4; i++) {
    servo[i].SetO(O[i]);
    servo[i].SetA(A[i]);
    servo[i].SetT(T);
    servo[i].SetPh(phase_diff[i]);
  }
  double ref=millis();
   for (double x=ref; x<=T*cycle+ref; x=millis()){
     for (int i=0; i<4; i++){
        servo[i].refresh();
     }
  }
}

//--------------------------------
//-- Zowi at rest position OLD
//--------------------------------
void Zowi::homeold()
{
  //-- All the parameters are set to 0
  //-- If the amplitudes are 0, there are no oscillation
  int A[4]= {0, 0, 0, 0};
  int O[4] = {0, 0, 0, 0};
  double phase_diff[4] = {0, 0, 0, 0};
  
  //-- Let's update the oscillators parameters!
  oscillateServos(A,O,500,phase_diff,1);
}

//--------------------------------
//-- Zowi at rest position
//--------------------------------
void Zowi::home()
{
  int homes[4]={90, 90, 90, 90}; //All the servos at rest position
  moveServos(500,homes);   //Move the servos in half a second
  
}

void Zowi::_execute(int A[4], int O[4], int T, double phase_diff[4], float steps = 1.0)
{
  int cycles=(int)steps;    

  //-- Execute complete cycles
  if (cycles >= 1) 
    for(int i = 0; i < cycles; i++) 
      oscillateServos(A,O, T, phase_diff);
      
  //-- Execute the final not complete cycle    
  oscillateServos(A,O, T, phase_diff,(float)steps-cycles);
}

//***********************************************************************************
//*********************************MOVEMENTS*****************************************
//***********************************************************************************

//------------------------------------------------
//-- Zowi gait: Walking  (forward or backward)    
//-- Parameters:
//--    * steps:  Number of steps
//--    * T : Period
//--    * Dir: Direction: FORWARD / BACKWARD
//------------------------------------------------
void Zowi::walk(float steps, int T, int dir)
{
  //-- Oscillator parameters for walking
  //-- Hip sevos are in phase
  //-- Feet servos are in phase
  //-- Hip and feet are 90 degrees out of phase
  //--      -90 : Walk forward
  //--       90 : Walk backward
  //-- Feet servos also have the same offset (for tiptoe a little bit)
  int A[4]= {30, 30, 20, 20};
  int O[4] = {0, 0, 4, -4};
  double phase_diff[4] = {0, 0, DEG2RAD(dir * -90), DEG2RAD(dir * -90)};

  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps);  
}

//----------------------------------------------------
//-- Zowi gait: Turning (left or right)
//-- Parameters:
//--   * Steps: Number of steps
//--   * T: Period
//--   * Dir: Direction: LEFT / RIGHT
//-----------------------------------------------------
void Zowi::turn(float steps, int T, int dir)
{
  //-- Same coordination than for walking (see Zowi::walk)
  //-- The Amplitudes of the hip's oscillators are not igual
  //-- When the right hip servo amplitude is higher, the steps taken by
  //--   the right leg are bigger than the left. So, the robot describes an 
  //--   left arc
  int A[4]= {30, 30, 20, 20};
  int O[4] = {0, 0, 4, -4};
  double phase_diff[4] = {0, 0, DEG2RAD(-90), DEG2RAD(-90)}; 
    
  if (dir == LEFT) {  
    A[0] = 30; //-- Left hip servo
    A[1] = 10; //-- Right hip servo
  }
  else {
    A[0] = 10;
    A[1] = 30;
  }
    
  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps); 
}


//------------------------------------------------------
//-- Zowi movement: up-down
//--   Zowi moves up and down
//--
//-- Parameters:
//--    * steps: Number of jumps
//--    * T: Period
//--    * h: Jump height: SMALL / MEDIUM / BIG 
//--              (or a number in degrees 0 - 90)
//----------------------------------------------------- 
void Zowi::updown(float steps, int T, int h)
{
  //-- Both feet are 180 degrees out of phase
  //-- Feet amplitude and offset are the same
  //-- Initial phase for the right foot is -90, so that it starts
  //--   in one extreme position (not in the middle)
  int A[4]= {0, 0, h, h};
  int O[4] = {0, 0, h, -h};
  double phase_diff[4] = {0, 0, DEG2RAD(-90), DEG2RAD(90)};
  
  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps); 
}

//-----------------------------------------------------------------
//-- Zowi gait: Moonwalker
//--   Zowi moves like Michael Jackson
//--
//--  Parameters:
//--    Steps: Number of steps
//--    T: Period
//--    h: Height. Typical valures between 15 and 40
//--    dir: Direction: LEFT / RIGHT
//------------------------------------------------------------------
void Zowi::moonwalker(float steps, int T, int h, int dir)
{
  //-- This motion is similar to that of the caterpillar robots: A travelling
  //-- wave moving from one side to another
  //-- The two Zowi's feet are equivalent to a minimal configuration. It is known
  //-- that 2 servos can move like a worm if they are 120 degrees out of phase
  //-- In the example of Zowi, the two feet are mirrored so that we have:
  //--    180 - 120 = 60 degrees. The actual phase difference given to the oscillators
  //--  is 60 degrees.
  //--  Both amplitudes are equal. The offset is half the amplitud plus a little bit of
  //-   offset so that the robot tiptoe lightly
 
  int A[4]= {0, 0, h, h};
  int O[4] = {0, 0, h/2+2, -h/2 -2};
  int phi = -dir * 90;
  double phase_diff[4] = {0, 0, DEG2RAD(phi), DEG2RAD(-60 * dir + phi)};
  
  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps); 
}

//---------------------------------------------------------
//-- Zowi movement: swinging side to side
//--
//--  Parameters:
//--     steps: Number of steps
//--     T : Period
//--     h : Amount of swing (from 0 to 50 aprox)
//-----------------------------------------------------------
void Zowi::swing(float steps, int T, int h)
{

  //-- Both feets are in phase. The offset is half the amplitude
  //-- It causes the robot to swing from side to side
  int A[4]= {0, 0, h, h};
  int O[4] = {0, 0, h/2, -h/2};
  double phase_diff[4] = {0, 0, DEG2RAD(0), DEG2RAD(0)};
  
  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps); 
}

//---------------------------------------------------------
//-- Zowi movement: swinging side to side without touching the floor with the heel
//--
//--  Parameters:
//--     steps: Number of steps
//--     T : Period
//--     h : Amount of swing (from 0 to 50 aprox)
//-----------------------------------------------------------
void Zowi::tiptoeSwing(float steps, int T, int h)
{
  //-- Both feets are in phase. The offset is not half the amplitude in order to tiptoe
  //-- It causes the robot to swing from side to side
  int A[4]= {0, 0, h, h};
  int O[4] = {0, 0, h, -h};
  double phase_diff[4] = {0, 0, 0, 0};
  
  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps); 
}




//-----------------------------------------------------------------
//-- Zowi gait: Crusaito: a mixture between moonwalker and walk
//--
//--   Parameters:
//--     steps: Number of steps
//--     T: Period
//--     h: height (Values between 20 - 50)
//--     dir:  Direction: LEFT / RIGHT
//------------------------------------------------------------------
void Zowi::crusaito(float steps, int T, int h, int dir)
{
  int A[4]= {25, 25, h, h};
  int O[4] = {0, 0, h/2+ 4, -h/2 - 4};
  double phase_diff[4] = {90, 90, DEG2RAD(0), DEG2RAD(-60 * dir)};
  
  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps); 
}

void Zowi::jump(float steps, int T){
  int up[]={90,90,150,30};
  moveServos(T,up);
  int down[]={90,90,90,90};
  moveServos(T,down);
}

//-----------------------------------------------------
//-- Zowi gait: Flapping
//--
//--  Parameters:
//--    steps: Number of steps
//--    T: Period
//--    h: height (Values between 10 - 30)
//--    dir: direction: FOREWARD, BACKWARD
//------------------------------------------------------
void Zowi::flapping(float steps, int T, int h, int dir)
{
  int A[4]= {12, 12, h, h};
  int O[4] = {0, 0, h - 10, -h + 10};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(180), DEG2RAD(-90 * dir), DEG2RAD(90 * dir)};
  
  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps); 
}


//-----------------------------------------------------
//-- Zowi gait: Shake a leg
//--
//--  Parameters:
//--    steps: Number of shakes
//--    T: Period of one shake
//--    dir: RIGHT=Right leg LEFT=Left leg
//------------------------------------------------------
void Zowi::shakeLeg (int steps,int T,int dir){

  //Parameters of all the movements. Default: Right leg
  int shake_leg1[4]={90, 90, 58, 35};   
  int shake_leg2[4]={90, 90, 58, 120};
  int shake_leg3[4]={90, 90, 58, 60};
  int homes[4]={90, 90, 90, 90};

  //Changes in the parameters if left leg is chosen
  if(dir==1)      
  {
    shake_leg1[2]=180-35;
    shake_leg1[3]=180-58;
    shake_leg2[2]=180-120;
    shake_leg2[3]=180-58;
    shake_leg3[2]=180-60;
    shake_leg3[3]=180-58;
  }
  
  //Time of the bend movement. Fixed parameter to avoid falls
  int T2=1000;    
  //Time of one shake, constrained in order to avoid movements too fast or too slow.            
  T=constrain(T, 150, 500);   

  //Bend movement
  moveServos(T2/2,shake_leg1);
  moveServos(T2/2,shake_leg2);
  
  //Shake movement
  for (int i=0;i<steps;i++)
  {
  moveServos(T/2,shake_leg3);
  moveServos(T/2,shake_leg2);
  }
  
  delay(T);
  //Return to home position
  moveServos(500,homes);
  
}

//-----------------------------------------------------
//-- Zowi gait: Lateral bend
//--
//--  Parameters:
//--    steps: Number of bends
//--    T: Period of one bend
//--    dir: RIGHT=Right bend LEFT=Left bend
//------------------------------------------------------

void Zowi::bend (int steps, int T, int dir)
{
  //Parameters of all the movements. Default: Left bend
  int bend1[4]={90, 90, 58, 35};
  int bend2[4]={90, 90, 58, 105};
  int homes[4]={90, 90, 90, 90};
  //Time of one bend, constrained in order to avoid movements too fast.
  T=max(T, 600);

  //Changes in the parameters if right direction is chosen 
  if(dir==-1)
  {
    bend1[2]=180-35;
    bend1[3]=180-58;
    bend2[2]=180-105;
    bend2[3]=180-58;
  }
  //Bend movement
  for (int i=0;i<steps;i++)
  {
  moveServos(T/4,bend1);
  moveServos(10,bend2);
  delay(3*T/4);
  moveServos(500,homes);
  }

}


//-----------------------------------------------------
//-- Zowi gait: Jitter 
//--
//--  Parameters:
//--    steps: Number of jitters
//--    T: Period of one jitter 
//--    h: height (Values between 5 - 25)   
//------------------------------------------------------

void Zowi::jitter(float steps, int T, int h)
{
  //-- Both feet are 180 degrees out of phase
  //-- Feet amplitude and offset are the same
  //-- Initial phase for the right foot is -90, so that it starts
  //--   in one extreme position (not in the middle)
  //-- h is constrained to avoid hit the feets
  h=min(25,h);
  int A[4]= {h, h, 0, 0};
  int O[4] = {0, 0, 0, 0};
  double phase_diff[4] = {DEG2RAD(-90), DEG2RAD(90), 0, 0};
  
  //-- Let's oscillate the servos!
  oscillateServos(A, O, T, phase_diff, steps); 
}


//-----------------------------------------------------
//-- Zowi gait: Jitter while up&down
//--
//--  Parameters:
//--    steps: Number of bends
//--    T: Period of one bend
//--    h: height (Values between 5 - 15) 
//------------------------------------------------------
void Zowi::ascendingTurn(float steps, int T, int h)
{
  //-- Both feet and legs are 180 degrees out of phase
  //-- Initial phase for the right foot is -90, so that it starts
  //--   in one extreme position (not in the middle)
  //-- h is constrained to avoid hit the feets
  h=min(15,h);
  int A[4]= {h, h, h, h};
  int O[4] = {0, 0, h+4, -h+4};
  double phase_diff[4] = {DEG2RAD(-90), DEG2RAD(90), DEG2RAD(-90), DEG2RAD(90)};
  
  //-- Let's oscillate the servos!
  _execute(A, O, T, phase_diff, steps); 
}



void Zowi::printMouth(int shape){
    Zowi::mouth.writeFull(mouthType[shape]);
}

void Zowi::clearMouth(){
    Zowi::mouth.clearMatrix();
}

double Zowi::getBattery(){
  return Zowi::battery.readBatPercent();
}

int Zowi::getDistance(){
  return Zowi::usSensor.read();
}