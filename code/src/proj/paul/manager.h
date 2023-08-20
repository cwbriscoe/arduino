// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef MANAGER_H
#define MANAGER_H

#include "lib/random.h"
#include "lib/thread.h"
#include "tasks.h"

/*
  PWM pins 3, 9, 10, 11 are 490hz
  PWM pins 5, 6 are 980hz

  duty cycle = required voltage / peak voltage
  analogWrite value = 255 * (desired voltage mV) / 5000mV

  step = 5000mV / 255 = 19.61mV

  min = 200mV, max = 800mV
  min = 11, max = 40
*/

namespace App {
const char blinkNm[] PROGMEM = "blink";
#ifdef DEBUG
const char counterNm[] PROGMEM = "counter";
#endif

class Manager;
static Manager* self;  // pointer to Manager instance to be used in static fuction callbacks

class Manager {
 private:
  SysLib::ThreadManager threads;  // pointer to our thread manager

  // task declarations
  LedBlinkTask ledBlinkTask;
  // TaskCount taskCount;

 public:
  Manager() {
    self = this;
  }

  void init() {
    // add tasks to the thread manager
    threads.add(blinkNm, 8, (unsigned long)500 * 1000, &ledBlinkTask);
#ifdef DEBUG
    // threads.add(counterNm, 9, (unsigned long)50 * 1000, &taskCount);
#endif

    // hook up callbacks
  }

  void run() {
    while (true) { threads.run(); }
  }
};
}  // namespace App

#endif