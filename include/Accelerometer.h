#pragma once
#include <Wire.h>

class Accelerometer {
public:
    void begin(TwoWire &wire = Wire);
    bool readAccel(float &ax, float &ay, float &az);
    bool readGyro(float &gx, float &gy, float &gz);
    bool initialized = false;
};