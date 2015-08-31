#include "US.h"

//****** US ******//
US::US(){
  _pinTrigger = 8;
  _pinEcho = 9;
  pinMode( _pinTrigger , OUTPUT );
  pinMode( _pinEcho , INPUT );
}
US::US(int pinTrigger=8, int pinEcho=9){
  _pinTrigger = pinTrigger;
  _pinEcho = pinEcho;
  pinMode( _pinTrigger , OUTPUT );
  pinMode( _pinEcho , INPUT );
}
long US::TP_init()
{
  digitalWrite(_pinTrigger, LOW);
  delayMicroseconds(2);
  digitalWrite(_pinTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(_pinTrigger, LOW);
  long microseconds = pulseIn(_pinEcho ,HIGH, 100000);
  return microseconds;
}
float US::read(){
  long microseconds = US::TP_init();
  long distance;
  distance = microseconds/29/2;
  if (distance == 0){
    distance = 999;
  }
  return distance;
}