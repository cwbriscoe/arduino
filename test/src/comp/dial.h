// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef DIAL_H
#define DIAL_H

#include "control.h"

class Dial : public Control {
 private:
  byte pin = 0;
  byte res = 0;
  int prevVal1 = 0;
  int prevVal2 = 0;
  int currVal = 0;
  void (*onValChangedCB)(const unsigned int);

 public:
  explicit Dial(const byte pin, const byte resolution) {
    this->pin = pin;
    this->res = resolution;
    assert(this->res <= 10);
    pinMode(this->pin, INPUT);
  }

  inline void update() {
    auto newVal = analogRead(pin) >> (10 - res);
    if (currVal != newVal && currVal != prevVal1 && currVal != prevVal2) {
      currVal = newVal;
      prevVal1 = currVal;
      prevVal2 = currVal;
      if (onValChangedCB) onValChangedCB(currVal);
      return;
    }
    prevVal2 = prevVal1;
    prevVal1 = newVal;
  }

  inline bool value() { return currVal; }

  inline void addOnValChangedCB(void (*cb)(const unsigned int)) { onValChangedCB = cb; }
};

class DialTask : public Dial, public Task {
 public:
  DialTask(const byte pin, const byte resolution) : Dial(pin, resolution) {}

  void run(const Time&) {
    update();
  }
};

#endif
