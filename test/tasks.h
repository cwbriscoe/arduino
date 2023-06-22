#ifndef TASKS_H
#define TASKS_H

#include "debug.h"
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
  //Serial.print("task 2 counter: ");
  //Serial.println(counter);
  //Serial.flush();
  counter++;
}

Trigger trigger(1000000, true);
void task3(const Time& time) {
  if (trigger.triggered(time)) {
    Serial.print("task 3 time: ");
    Serial.println(time.micros());
  }
}

#endif