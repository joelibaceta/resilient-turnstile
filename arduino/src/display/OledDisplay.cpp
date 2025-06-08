#include "OledDisplay.h"
#include <stdarg.h>
#include <string.h>   // para strstr
#include <stdio.h>    // para snprintf
#include <stdlib.h>   // para dtostrf

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

OledDisplay::OledDisplay()
  : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

bool OledDisplay::begin() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) return false;
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("OLED OK 😊"));
  display.display();
  delay(1000);
  return true;
}

void OledDisplay::showMessage(const char* msg) {
  clear();
  display.setTextSize(2);
  display.println(msg);
  display.display();
}

void OledDisplay::clear() {
  display.clearDisplay();
  display.setCursor(0, 0);
}

void OledDisplay::printFormatted(const char* fmt, ...) {
  char buffer[64];

#if defined(__AVR__)
  // Versión especial para AVR (como Arduino Uno), sin soporte %f en vsnprintf
  if (strstr(fmt, "%f") != nullptr || strstr(fmt, "%.1f") != nullptr || strstr(fmt, "%.2f") != nullptr) {
    va_list args;
    va_start(args, fmt);

    float f1 = va_arg(args, double);  // float se promueve a double en va_arg
    float f2 = va_arg(args, double);

    char s1[10], s2[10];
    dtostrf(f1, 4, 1, s1);
    dtostrf(f2, 4, 1, s2);

    // Modifica aquí según tu formato deseado
    snprintf(buffer, sizeof(buffer), "T:%s C H:%s", s1, s2);

    va_end(args);
  } else
#endif
  {
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
  }

  display.setTextSize(2);
  display.print(buffer);
  display.display();
}