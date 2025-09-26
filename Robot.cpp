#include "MeGyro.h"
#include "MePort.h"
#include "MeUltrasonicSensor.h"
#include "Robot.h"

Robot::Robot(){
  _capteur = MeUltrasonicSensor(PORT_10);
  _led = MeRGBLed( PORT0, LEDNUM );
  _gyro = MeGyro(0, 0x69);
  _led.setpin(LEDPIN);
}
void Robot::setup(){
  _gyro.begin();
}
void Robot::update(){
  const int rateDistance=100;
  const int rateAffichage=250;
  static unsigned long _previousTime =0;
  static unsigned long _previousTime2 =0;

  _currentTimes = millis();

 if (_currentTimes - _previousTime > rateDistance){
    _previousTime = _currentTimes;
   distance = _capteur.distanceCm();
    _gyro.update();
 }
 if (_currentTimes - _previousTime2 > rateAffichage){
  _previousTime2 = _currentTimes;
  Serial.println(distance);
 }
    _moteur.update();
    switch (_state) {
      case NORMAL:
        NormalStateOn();
        break;
      case RALENTI:
        RalentiStateOn();
        break;
      case DANGER:
        DangerStateOn();
        break;
  }
  
}

void Robot::NormalStateOn(){
  static unsigned long lastTime = 0;
  const int rate = 500;

  static bool firstTime = true;
  // Code d'ENTRÉE
  if (firstTime) {
    // Code d'initialisation de l'état
    LedTask('g');
    firstTime = false;
    Serial.println("Entrer États : Normal");
    lastTime = _currentTimes;

    return;
  }

  // Code d'EXÉCUTION de l'état
  _moteur.avancer();
  // Code de TRANSITION
  bool transDanger = (distance<40);
  bool transRalenti = (distance<80 && distance > 40);;

  if (transDanger) {
    // Code de SORTIE
    firstTime = true;
    Serial.println("Sorti État : Danger");
   _state = DANGER;
    return;
  }
  // Il est possible d'avoir plusieurs transitions
  if (transRalenti) {
    // Code de SORTIE
    firstTime = true;
    Serial.println("Sorti État : Ralenti");
    _state = RALENTI;
    return;
  }
}void Robot::DangerStateOn(){
  static unsigned long lastTime = 0;
  static unsigned long lastTime2 = 0;
  static int gyro_Position = _gyro.getAngleZ();
  const int stop = 500;
  const int rate2 = 1000;
  static bool firstTime = true;

  // Code d'ENTRÉE
  if (firstTime) {
    firstTime = false;
    Serial.println("Entrer États : Danger");
    LedTask('r');
    lastTime  = _currentTimes;
    lastTime2 = _currentTimes + 500;
    gyro_Position = _gyro.getAngleZ();      // snapshot du yaw au début
  }

  _moteur.arreter();
  if (_currentTimes - lastTime < stop) return;

  // Code d'EXÉCUTION de l'état
  _moteur.reculer();

  // Code de TRANSITION
  if (_currentTimes - lastTime2 < rate2) return;

  _moteur.gauche();                        
  int dYawCW = gyro_Position - (int)_gyro.getAngleZ();
  dYawCW = (dYawCW % 360 + 360) % 360;
  if (dYawCW < 85) return;
                



  bool transNormal  = (distance > 80);
  bool transRalenti = (distance < 80 && distance > 40);

  if (transNormal) {
    firstTime = true;
    Serial.println("Sorti État : Normal");
    _state = NORMAL;
    return;
  }
  if (transRalenti) {
    firstTime = true;
    Serial.println("Sorti État : Ralenti");
    _state = RALENTI;
    return;
  }
}

void Robot::RalentiStateOn(){

 static unsigned long lastTime = 0;
  const int rate = 500;

  static bool firstTime = true;

  // Code d'ENTRÉE
  if (firstTime) {
    // Code d'initialisation de l'état
    LedTask('b');
    firstTime = false;
    Serial.println("Entrer États : Ralenti");
  }

  if (_currentTimes - lastTime < rate) return;

  lastTime = _currentTimes;

  // Code d'EXÉCUTION de l'état
  _moteur.ralentir();
  // Code de TRANSITION
  bool transNormal = (distance>80);
  bool transDanger = (distance<40);

  if (transNormal) {
    // Code de SORTIE
    firstTime = true;
    Serial.println("Sorti État : Normal");
   _state = NORMAL;
    return;
  }
  // Il est possible d'avoir plusieurs transitions
  if (transDanger) {
    // Code de SORTIE
    firstTime = true;
    Serial.println("Sorti État : Danger");
    _state = DANGER;
    return;
  }
}
void Robot::LedTask(char couleur){
  _led.setColor (0,0, 0, 0); 
  _led.show(); 

  switch(couleur){
    case 'r':
      _led.setColor (0,10, 0, 0); 
      break;
    case 'g':
      for(int i=1; i<=6; i++){
      _led.setColor (i,0, 10, 0); 
        }
      break;
    case 'b':
      for(int i=1; i<=6; i++){
      _led.setColor (i,0, 0, 10); 
        }
      break;
  }
  _led.show();

}
