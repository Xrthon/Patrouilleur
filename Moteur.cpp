#include "Moteur.h"

// Définition du pointeur d’instance
Moteur* Moteur::s_instance = nullptr;

Moteur::Moteur()
: Encoder_1(SLOT1), Encoder_2(SLOT2)
{
}

void Moteur::setup()
{
  // Enregistre l’instance pour les ISR statiques
  s_instance = this;

  // Attache les interruptions d’encodeurs
  attachInterrupt(Encoder_1.getIntNum(), Moteur::isr_process_encoder1, RISING);
  attachInterrupt(Encoder_2.getIntNum(), Moteur::isr_process_encoder2, RISING);

  Serial.begin(115200);

  // PWM ~8 KHz (protégé selon MCU)
  #if defined(TCCR1A) && defined(TCCR1B)
    TCCR1A = _BV(WGM10);
    TCCR1B = _BV(CS11) | _BV(WGM12);
  #endif
  #if defined(TCCR2A) && defined(TCCR2B)
    TCCR2A = _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS21);
  #endif

  // Réglages moteurs/encodeurs
  Encoder_1.setPulse(9);
  Encoder_2.setPulse(9);
  Encoder_1.setRatio(39.267);
  Encoder_2.setRatio(39.267);
  Encoder_1.setPosPid(1.8, 0, 1.2);
  Encoder_2.setPosPid(1.8, 0, 1.2);
  Encoder_1.setSpeedPid(0.18, 0, 0);
  Encoder_2.setSpeedPid(0.18, 0, 0);
}

void Moteur::update()
{
  // Pilotage selon l’état courant
  switch (_state)
  {
    case AVANCE:
      Encoder_1.setTarPWM(-fullspeed);
      Encoder_2.setTarPWM(fullspeed);
      break;

    case GAUCHE:
      Encoder_1.setTarPWM(-fullspeed);
      Encoder_2.setTarPWM(-halfspeed);
      break;

    case DROIT:
      Encoder_1.setTarPWM(fullspeed);
      Encoder_2.setTarPWM(fullspeed);
      break;

    case RECUL:
      Encoder_1.setTarPWM(fullspeed);
      Encoder_2.setTarPWM(-fullspeed);
      break;

    case RALENTI:
      Encoder_1.setTarPWM(-halfspeed);
      Encoder_2.setTarPWM(halfspeed);
      break;
    case ARRET:
    default:
      // Maintient la position et vitesse nulle
      Encoder_1.moveTo(Encoder_1.getCurPos(), 0);
      Encoder_2.moveTo(Encoder_2.getCurPos(), 0);
      break;
  }

  // IMPORTANT: appeler loop() des drivers à chaque tour
  Encoder_1.loop();
  Encoder_2.loop();

  //
}

void Moteur::setState(State s)
{
  _state = s;
}

// --- ISR wrappers statiques ---
void Moteur::isr_process_encoder1()
{
  if (s_instance) s_instance->handleEncoder1ISR();
}

void Moteur::isr_process_encoder2()
{
  if (s_instance) s_instance->handleEncoder2ISR();
}

void Moteur::handleEncoder1ISR()
{
  if (digitalRead(Encoder_1.getPortB()) == 0)
    Encoder_1.pulsePosMinus();
  else
    Encoder_1.pulsePosPlus();
}

void Moteur::handleEncoder2ISR()
{
  if (digitalRead(Encoder_2.getPortB()) == 0)
    Encoder_2.pulsePosMinus();
  else
    Encoder_2.pulsePosPlus();
}
