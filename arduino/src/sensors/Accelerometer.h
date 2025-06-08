#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <Arduino.h>

void initAccelerometer();
void readAccel(float &x, float &y, float &z);
void readGyro(float &x, float &y, float &z);
bool isVertical();  

#endif