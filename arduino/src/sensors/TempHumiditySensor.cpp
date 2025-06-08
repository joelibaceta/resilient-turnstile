#include "TempHumiditySensor.h"
#include <DHT.h>

#define DHTTYPE DHT11  // Puedes cambiarlo por DHT22 si lo usas

static DHT dht(0, DHTTYPE);  // Se sobreescribirá el pin en init

void initTempHumiditySensor(uint8_t pin) {
  dht = DHT(pin, DHTTYPE);
  dht.begin();
}

float readTemperature() {
  return dht.readTemperature();
}

float readHumidity() {
  return dht.readHumidity();
}