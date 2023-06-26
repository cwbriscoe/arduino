// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef BUTTON_H
#define BUTTON_H

#include "control.h"

class Button : public Control {
 private:
  byte pin = 0;
  bool prevState = false;
  bool currState = false;

 public:
  explicit Button(const byte pin) {
    this->pin = pin;
    pinMode(this->pin, INPUT);
  }

  inline void update() {
    currState = digitalRead(pin) == HIGH;
    if (currState != prevState) {
      prevState = currState;
      notifyListeners(currState);
    }
  }

  inline bool value() {
    return currState;
  }

  inline bool isOn() {
    return currState == HIGH;
  }

  inline bool isOff() {
    return currState == LOW;
  }
};

class ButtonTask : public Button, public Task {
 public:
  ButtonTask(const byte pin) : Button(pin) {}

  void run() {
    update();
  }
};

#endif
