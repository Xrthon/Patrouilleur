#include "WString.h"
#include <avr/interrupt.h>
#include <MeAuriga.h>

#pragma once
#include <HCSR04.h>

class Capteur{

public:
  enum State {
    TO_CLOSE,
    IN_RANGE,
    TO_FAR
  };

  Capteur();

  void update();
  void setup(int minDist, int maxDist);
  void setMaxDist(int maxDist);
  void setMinDist(int minDist);
  void setRate(int rate);
  void setCapteurState();
  int getPinEcho() {return _PIN_ECHO;};
  int getPinTrig() {return _PIN_TRIG;};
  int getMaxDist() {return _maxDist;};
  int getMinDist() {return _minDist;};
  int getDistance() {return _distance;};
  String getDistanceStr() {return ("Distance: "+ String(_distance) + " cm");};
  State getState() {return _state;};
  String setSensorState();
  


  private:
    unsigned long _currentTime = 0;
    double _distance = 70;
    String _sensorState;
    MeUltrasonicSensor _capteur;
    int distance;
    State _state= TO_CLOSE;
    byte _PIN_ECHO,_PIN_TRIG;
      int  _minDist = 10,_maxDist = 50,_rate =1000;
};
