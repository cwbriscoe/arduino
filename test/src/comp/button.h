// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef BUTTON_H
#define BUTTON_H

#include "control.h"

class Button : public Control {
 private:
  byte pin = 0;

 public:
  explicit Button(const byte pin) {
    this->pin = pin;
    pinMode(this->pin, INPUT);
  }

  inline bool value() {
    return digitalRead(pin);
  }

  inline bool isOn() {
    return this->value() == HIGH;
  }

  inline bool isOff() {
    return this->value() == LOW;
  }
};

#endif
