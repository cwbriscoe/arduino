// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef MANAGER_H
#define MANAGER_H

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

  min = 11 * 19.61mV = 215.71mV
  max = 40 * 19.61mV = 784.4mV

  A 10K OHM resister and a 10-100uF capacitor low-pass filter should drop voltage a bit
  so min and max may need to be raised slightly to stay close to the 200-800mV range

  Maybe a DAC like MCP4725 will work better than a low-pass filter?
*/

namespace App {
// configurable constants
const bool randomOutput = false;
const int minDutyCycle = 11;
const int maxDutyCycle = 40;
const unsigned long periodLength = 500000;  // 500 milliseconds

const char pwmNm1[] PROGMEM = "pwm1";
const char pwmNm2[] PROGMEM = "pwm2";

class Manager;
static Manager* self;  // pointer to Manager instance to be used in static fuction callbacks

class Manager {
  const byte pwmPin1 = 5;
  const byte pwmPin2 = 6;

 private:
  SysLib::ThreadManager threads;  // pointer to our thread manager

  // task declarations
  PWMTask pwmTask1;
  PWMTask pwmTask2;

 public:
  Manager() : pwmTask1(pwmPin1, minDutyCycle, maxDutyCycle, randomOutput),
              pwmTask2(pwmPin2, minDutyCycle, maxDutyCycle, randomOutput) {
    self = this;
  }

  void init() {
    // calculate the update interval required
    int updatesPerPeriod = (maxDutyCycle - minDutyCycle + 1) * 2 - 1;
    unsigned long updateInterval = (periodLength / updatesPerPeriod);

    // add tasks to the thread manager
    threads.add(pwmNm1, 1, updateInterval, &pwmTask1);
    threads.add(pwmNm2, 1, updateInterval, &pwmTask2);
  }

  void run() {
    while (true) { threads.run(); }
  }
};
}  // namespace App

#endif