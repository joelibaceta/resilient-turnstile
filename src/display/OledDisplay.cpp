#include "OledDisplay.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

OledDisplay::OledDisplay()
  : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {}

bool OledDisplay::begin() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("❌ OLED no detectado"));
    return false;
  }
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
  display.setTextSize(1);
  display.println(msg);
  display.display();
}