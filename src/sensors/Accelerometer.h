#ifndef MPU6050_SENSOR_H
#define MPU6050_SENSOR_H

#include <MPU6050.h>
#include <Wire.h>

class MPU6050Sensor {
  public:
    void begin();
    void readAcceleration(int16_t& ax, int16_t& ay, int16_t& az);
    void readGyroscope(int16_t& gx, int16_t& gy, int16_t& gz);

  private:
    MPU6050 mpu;
};

#endif