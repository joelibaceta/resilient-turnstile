#ifndef TEMP_HUMIDITY_SENSOR_H
#define TEMP_HUMIDITY_SENSOR_H

#include <Arduino.h>

void initTempHumiditySensor(uint8_t pin);
float readTemperature();
float readHumidity();

#endif