// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef TASK_H
#define TASK_H

#include "time.h"

class Task {
private:
#ifdef DEBUG
  unsigned long runTime = 0;  // accumulative run time of the task
#endif

public:
  Task() {}

  void setup() {
    this->init();
  }

  void exec(const Time& time) {
#ifdef DEBUG
    auto start = micros();
#endif

    this->run(time);

#ifdef DEBUG
    this->runTime += micros() - start;
#endif
  }

  virtual void init() {}

  virtual void run(const Time&) {}

#ifdef DEBUG
  unsigned long getRunTime() {
    return this->runTime;
  }

  void resetStats() {
    this->runTime = 0;
  }
#endif
};

#endif