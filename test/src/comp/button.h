// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef BUTTON_H
#define BUTTON_H

#include "env.h"

class Button {
 private:
  byte pin = 0;

 public:
  explicit Button(const byte pin) {
    this->pin = pin;
    pinMode(this->pin, INPUT);
  }

  bool value() {
    return digitalRead(pin);
  }

  bool isOn() {
    return this->value() == HIGH;
  }

  bool isOff() {
    return this->value() == LOW;
  }
};

#endif
