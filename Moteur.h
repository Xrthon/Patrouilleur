#pragma once
#include <Arduino.h>
#include "MeAuriga.h"

class Moteur {
public:
  enum State { ARRET, AVANCE, GAUCHE, DROIT, RECUL, RALENTI };

  Moteur();                 // construit les 2 encodeurs sur SLOT1/SLOT2
  void setup();             // attache les interruptions, PID, PWM, etc.
  void update();            // à appeler dans loop()
  void setState(State s);   // change l’état demandé

  // (facultatif) getters debug
  long speed1() const { return Encoder_1.getCurrentSpeed(); }
  long speed2() const { return Encoder_2.getCurrentSpeed(); }
  long pos1()   const { return Encoder_1.getCurPos(); }
  long pos2()   const { return Encoder_2.getCurPos(); }
  
  void avancer() { setState(AVANCE); }
  void reculer() { setState(RECUL); }
  void gauche()  { setState(GAUCHE); }
  void droite()  { setState(DROIT); }
  void arreter() { setState(ARRET); }
  void ralentir() { setState(RALENTI);}


private:
  static void isr_process_encoder1();
  static void isr_process_encoder2();
 int fullspeed = 255;
 int halfspeed = 122;
 
  void handleEncoder1ISR();
  void handleEncoder2ISR();

  static Moteur* s_instance;

  MeEncoderOnBoard Encoder_1;  // SLOT1
  MeEncoderOnBoard Encoder_2;  // SLOT2
  State _state = ARRET;
};
