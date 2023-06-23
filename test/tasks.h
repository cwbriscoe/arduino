#ifndef TASKS_H
#define TASKS_H

#include "debug.h"
#include "led.h"

auto blinkStatus = false;
void taskBlink(const Time&) {
  if (blinkStatus) {
    powerOnLED(LED_BUILTIN);
  } else {
    powerOffLED(LED_BUILTIN);
  }

  blinkStatus = !blinkStatus;
}

Trigger trigger((unsigned long)1000 * 1000, true);
void taskCount(const Time& time) {
  if (trigger.triggered(time)) {
    Serial.print("taskCount time: ");
    Serial.println(time.microseconds());
  }
}

#endif