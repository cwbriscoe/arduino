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

unsigned long nextTime;
unsigned long prevTime;
bool firstTime = true;
bool nextOverflow = false;
bool timeOverflow = false;

void task3(unsigned long time) {
  if (firstTime) {
    nextTime = time;
    prevTime = time;
    firstTime = false;
    return;
  }

  if (prevTime > time) {
    timeOverflow = true;
  }

  prevTime = time;

  if (timeOverflow && nextOverflow) {
    timeOverflow = false;
    nextOverflow = false;
  }

  if (timeOverflow && !nextOverflow) {
    Serial.print("task 3 time(overflow): ");
    Serial.println(time);
    Serial.println(nextTime);
    Serial.flush();
    if (MAX_TIME - nextTime < 1000000) {
      nextTime = 1000000 - (MAX_TIME - nextTime);
      timeOverflow = false;
      return;
    }
    nextTime += 1000000;
    timeOverflow = false;
    return;
  }

  if (!nextOverflow && nextTime <= time) {
    Serial.print("task 3 time: ");
    Serial.println(time);
    Serial.flush();
    if (MAX_TIME - nextTime < 1000000) {
      nextTime = 1000000 - (MAX_TIME - nextTime);
      nextOverflow = true;
      return;
    }
    nextTime += 1000000;
    return;
  }
}

#endif