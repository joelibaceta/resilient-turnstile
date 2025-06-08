#ifndef IMPACT_SENSOR_H
#define IMPACT_SENSOR_H

#include <Arduino.h>

void initImpactSensor(uint8_t pin);
bool detectImpact();

#endif