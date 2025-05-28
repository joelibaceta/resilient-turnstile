#include "Accelerometer.h"
#include <MPU6050.h>

static MPU6050 mpu(0x68, (void*)&Wire);  // dirección I2C por defecto, con Wire casteado a void*

void Accelerometer::begin(TwoWire &wire) {
    bool encontrado = false;
    wire.begin();
    mpu.initialize();
    initialized = mpu.testConnection();
}

bool Accelerometer::readAccel(float &ax, float &ay, float &az) {
    if (!initialized) return false;

    int16_t rawX, rawY, rawZ;
    mpu.getAcceleration(&rawX, &rawY, &rawZ);

    ax = rawX / 16384.0f;
    ay = rawY / 16384.0f;
    az = rawZ / 16384.0f;

    return true;
}

bool Accelerometer::readGyro(float &gx, float &gy, float &gz) {
    if (!initialized) return false;

    int16_t rawX, rawY, rawZ;
    mpu.getRotation(&rawX, &rawY, &rawZ);

    gx = rawX / 131.0f;
    gy = rawY / 131.0f;
    gz = rawZ / 131.0f;

    return true;
}