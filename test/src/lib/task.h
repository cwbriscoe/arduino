// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef TASK_H
#define TASK_H

#include "time.h"

namespace SysLib {
class Task {
 private:
#ifdef DEBUG
  unsigned long runTime = 0;  // accumulative run time of the task
#endif

 public:
  Task() {}

  void exec(const Time& time) {
#ifdef DEBUG
    auto start = micros();
#endif

    this->run(time);

#ifdef DEBUG
    this->runTime += micros() - start;
#endif
  }

  virtual void run(const Time&) {}

#ifdef DEBUG
  unsigned long getRunTime() const {
    return this->runTime;
  }

  void resetStats() {
    this->runTime = 0;
  }
#endif
};
}  // namespace SysLib

#endif