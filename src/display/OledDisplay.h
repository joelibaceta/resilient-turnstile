#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

class OledDisplay {
  public:
    OledDisplay();
    bool begin();
    void showMessage(const char* msg);

  private:
    Adafruit_SSD1306 display;
};

#endif