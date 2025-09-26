
#pragma once
#include "Moteur.h"
#include "Capteur.h"
enum  State{NORMAL,RALENTI,DANGER,STOP};

class Robot{

  public:
  Robot();
  void update();
  void patrouilleMode(Moteur moteur);
  void NormalStateOn();
  void RalentiStateOn();
  void DangerStateOn();
  void NormalTask();
  void RalentiTask();
  void DangerTask();
  void setup();
  void setColor(MeRGBLed led);
  void LedTask(char couleur);



  private:
  const int LEDNUM = 12;
  const int LEDPIN = 44;
  const int RINGALLLEDS = 0;

  MeRGBLed _led;
  State _state;
  MeUltrasonicSensor _capteur;
  MeGyro _gyro;
  int distance;
  Moteur _moteur;

  int nbPulseRot = 9;
  float diametreRoue = 6.5;
  float ratio = 39.267;
  float oneRot = nbPulseRot * ratio;
  float pulsePerMeter = (100 / diametreRoue) * oneRot;
  int _rate = 100;
  unsigned long _currentTimes;
};