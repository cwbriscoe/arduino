// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef BUTTON_H
#define BUTTON_H

#include "inc/control.h"

namespace SysLib {
class Button : public Control {
 private:
  byte pin = 0;
  bool prevState = false;
  bool currState = false;
  void (*onBtnDownCB)() = nullptr;
  void (*onBtnUpCB)() = nullptr;

 public:
  explicit Button(const byte pin) {
    this->pin = pin;
    pinMode(this->pin, INPUT);
  }

  inline void update() {
    currState = (digitalRead(pin) == HIGH);
    if (currState != prevState) {
      prevState = currState;
      if (onBtnDownCB && currState == HIGH) {
        onBtnDownCB();
        return;
      }
      if (onBtnUpCB && currState == LOW)
        onBtnUpCB();
    }
  }

  inline bool value() const { return currState; }

  inline bool isOn() const { return currState == HIGH; }
  inline bool isOff() const { return currState == LOW; }

  inline void addOnBtnDownCB(void (*cb)()) { onBtnDownCB = cb; }
  inline void addOnBtnUpCB(void (*cb)()) { onBtnUpCB = cb; }
};

class ButtonTask : public Button, public Task {
 public:
  explicit ButtonTask(const byte pin) : Button(pin) {}

  void run(const Time&) override {
    update();
  }
};
}  // namespace SysLib

#endif
