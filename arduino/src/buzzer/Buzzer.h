#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

void initBuzzer(uint8_t pin);
void buzzerClick(); // breve "tap"
void buzzerTone(unsigned int frequency, unsigned int duration);
void buzzerDoubleBeep();
void buzzerAlert();

#endif