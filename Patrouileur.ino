
#include "Robot.h"

  Robot robotPatrouilleur;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  robotPatrouilleur.setup();
}

void loop() {
  robotPatrouilleur.update();
}
