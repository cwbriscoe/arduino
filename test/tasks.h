#ifndef TASKS_H
#define TASKS_H

#include "env.h"
#include "led.h"

auto blinkStatus = false;
void task1(unsigned long) {
  if (blinkStatus) {
    powerOnLED(LED_BUILTIN);
  } else {
    powerOffLED(LED_BUILTIN);
  }

  blinkStatus = !blinkStatus;
}

auto counter = 0;
void task2(unsigned long) {
  Serial.print("task 2 counter: ");
  Serial.println(counter);
  Serial.flush();
  counter++;
}

#endif