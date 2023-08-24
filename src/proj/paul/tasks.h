// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef TASKS_H
#define TASKS_H

#include "lib/task.h"

namespace App {
/*****************************************************************************/
/* PWM Task                                                                  */
/*****************************************************************************/
class PWMTask : public SysLib::Task {
 private:
  byte pin;
  int minDutyCycle;
  int maxDutyCycle;
  bool rand;
  int val;
  int step;

#ifdef DEBUG
  int outRate = 100;
  int curCount = 0;
#endif

 public:
  PWMTask(const byte pin, const int minDutyCycle, const int maxDutyCycle, bool rand) {
    this->pin = pin;
    this->minDutyCycle = minDutyCycle;
    this->maxDutyCycle = maxDutyCycle;
    this->rand = rand;

    val = random(this->minDutyCycle, this->maxDutyCycle + 1);

    step = random(0, 2);
    if (step == 0) step = -1;
  }

  void run(const SysLib::Time&) final {
    if (rand) {
      step = random(0, 3);
      if (step == 2) step = -1;
    }

    if (val > maxDutyCycle) step = -1;
    if (val < minDutyCycle) step = +1;
    val += step;

    analogWrite(pin, val);

#ifdef DEBUG
    curCount++;
    if (curCount == outRate) {
      print(F("pin: "));
      print(pin);
      print(F(", val: "));
      println(val);
      curCount = 0;
    }
#endif
  }
};
}  // namespace App

#endif