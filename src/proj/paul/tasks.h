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
  int val;
  int step;

 public:
  PWMTask(const byte pin, const int minDutyCycle, const int maxDutyCycle) {
    this->pin = pin;
    this->minDutyCycle = minDutyCycle;
    this->maxDutyCycle = maxDutyCycle;

    val = random(this->minDutyCycle, this->maxDutyCycle + 1);

    step = random(0, 2);
    if (step == 0) step = -1;
  }

  void run(const SysLib::Time& time) final {
    if (val >= maxDutyCycle) step = -1;
    if (val <= minDutyCycle) step = +1;
    val += step;
  }
};
}  // namespace App

#endif