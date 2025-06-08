#include "Accelerometer.h"
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void initAccelerometer() {
    Wire.begin();
    mpu.initialize();
}

void readAccel(float &x, float &y, float &z) {
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);
    x = ax / 16384.0f;
    y = ay / 16384.0f;
    z = az / 16384.0f;
}

void readGyro(float &x, float &y, float &z) {
    int16_t gx, gy, gz;
    mpu.getRotation(&gx, &gy, &gz);
    x = gx / 131.0f;
    y = gy / 131.0f;
    z = gz / 131.0f;
}

bool isVertical() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Normalizamos Z (valores entre -1 y 1)
  float zNorm = az / 16384.0;  // 1g ~ 16384 en el MPU6050
  return zNorm >= 0.9;
}