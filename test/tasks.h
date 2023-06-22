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
    nextTime = time + 1000000;
    prevTime = time;
    firstTime = false;
    return;
  }

  //Serial.println(time);

  if (prevTime > time) {
    timeOverflow = true;
    //Serial.println("1-timeOverflow = true");
  }
  prevTime = time;

  if (timeOverflow && nextOverflow) {
    timeOverflow = false;
    nextOverflow = false;
  }

  if (timeOverflow && !nextOverflow) {
    Serial.print("task 3 time(overflow): ");
    Serial.println(time);
    //Serial.println(nextTime);
    //Serial.flush();
    if (MAX_TIME - nextTime <= 1000000) {
      nextTime = 1000000 - (MAX_TIME - nextTime);
      nextOverflow = true;
      //Serial.println("1-nextOverflow = true");
      return;
    }
    nextTime += 1000000;
    return;
  }

  //if (timeOverflow) {
  //  Serial.println("2-timeOverflow = true");
  //}

  if (!nextOverflow && nextTime <= time) {
    Serial.print("task 3 time: ");
    Serial.println(time);
    //Serial.print(" timeOverflow: ");
    //Serial.println(timeOverflow);
    if (MAX_TIME - nextTime <= 1000000) {
      nextTime = 1000000 - (MAX_TIME - nextTime);
      nextOverflow = true;
      //Serial.println("2-nextOverflow = true");
      return;
    }
    nextTime += 1000000;
    return;
  }
}

#endif