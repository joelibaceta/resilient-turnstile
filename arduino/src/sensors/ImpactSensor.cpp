#include "ImpactSensor.h"

static uint8_t impactPin;

void initImpactSensor(uint8_t pin) {
  impactPin = pin;
  pinMode(impactPin, INPUT_PULLUP);
}


bool detectImpact() {
  return digitalRead(impactPin) == LOW;
}