#include "Buzzer.h"

static uint8_t buzzerPin;

void initBuzzer(uint8_t pin) {
  buzzerPin = pin;
  pinMode(buzzerPin, OUTPUT);
}

void buzzerClick() {
  digitalWrite(buzzerPin, HIGH);
  delay(50);
  digitalWrite(buzzerPin, LOW);
}

void buzzerTone(unsigned int frequency, unsigned int duration) {
  tone(buzzerPin, frequency, duration);
  delay(duration);
  noTone(buzzerPin);  // libera el pin
}

void buzzerDoubleBeep() {
  buzzerTone(1000, 100);  // 1000 Hz por 100 ms
  delay(100);
  buzzerTone(1000, 100);
}

void buzzerAlert() {
  for (int i = 0; i < 3; ++i) {
    buzzerTone(800, 300);
    delay(200);
    buzzerTone(800, 300);
    delay(600);
  }
}