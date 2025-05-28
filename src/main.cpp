#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Accelerometer.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Accelerometer accel;

void setup() {
  Serial.begin(115200);
  delay(1000);
  

  Serial.println(F("Iniciando OLED I2C en D4/D5..."));

  // D4 = GPIO 4, D5 = GPIO 5
  Wire.begin(4, 5);

  accel.begin(Wire);


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED no detectada 😢"));
    while (true);
  }


  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(("Iniciando "));
  display.display();

  delay(1000);


  if (accel.initialized) {
    Serial.println(F("Acelerómetro inicializado correctamente 😊"));
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("SI ACC"));
    display.display();
  } else {
    Serial.println(F("Error al inicializar el acelerómetro 😢"));
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(F("NO ACC"));
    display.display();
  }

  delay(1000);

  bool encontrado = false;
  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("Encontrado dispositivo I2C en 0x");
      Serial.println(address, HEX);

      display.setCursor(0, 16 + (encontrado * 10));
      display.print("0x");
      display.println(address, HEX);
      display.display();

      encontrado++;
      delay(250);
    }
  }
  
  delay(1000);

}

void loop() {
  float ax, ay, az;

  if (!accel.readAccel(ax, ay, az)) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("Error de lectura 😢"));
    display.display();
    return;
  } else {
    Serial.printf("Aceleración: X=%.2f, Y=%.2f, Z=%.2f\n", ax, ay, az);
  }

  delay(1000);
}