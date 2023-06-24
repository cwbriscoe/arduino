// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef TASK_H
#define TASK_H

#include "time.h"

class Task {
public:
  Task() {}

  void setup() {
    this->init();
  }
  
  void exec(const Time& time) {
    this->run(time);
  }

  virtual void init() {}

  virtual void run(const Time&) {}
};

#endif