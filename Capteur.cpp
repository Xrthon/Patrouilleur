#include "HardwareSerial.h"
#include "Arduino.h"
#include "Capteur.h"
#include <HCSR04.h>
// Assure-toi que Arduino.h est inclus dans ton .hpp ou ici si besoin


Capteur::Capteur()
{
 _capteur = MeUltrasonicSensor(PORT_7);
}
void Capteur::setup(int minDistance, int maxDistance) {
  Serial.println("Setup complet [ Capteur ]");
  _minDist = minDistance;
  _maxDist = maxDistance;

  setMinDist(_minDist);
  setMaxDist(_maxDist);
  Serial.println("Setup complet [ TEST cap ]");

}


void Capteur::setMaxDist(int maxDist) {
  if (maxDist < _minDist) return;
  _maxDist = maxDist;
}

void Capteur::setMinDist(int minDist) {
  if (minDist > _maxDist) return;
  _minDist = minDist;
}

void Capteur::setRate(int rate){
  _rate = rate; // en ms
}

void Capteur::setCapteurState(){
  static unsigned long _previousTime = 0;
  static int lastDistance = 0;

  _currentTime = millis();
  if (_currentTime - _previousTime < _rate) return;
  _previousTime = _currentTime;
  _distance = _capteur.distanceCm();

  if (_distance <= 0 ||  _distance >= 400) {
    _distance = lastDistance;
  } else {
    lastDistance = _distance;
  }
  if (_distance < _minDist) {
    _state = TO_CLOSE;
  } else if (_distance > _maxDist) {
    _state = TO_FAR;

  } else {
    _state = IN_RANGE;
  }

}

void Capteur::update(){
  setCapteurState();
}
