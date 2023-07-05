// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef LED_H
#define LED_H

#include "inc/control.h"

namespace SysLib {
class Led : public Control {
 private:
  byte pin = 0;
  bool value = false;

 public:
  explicit Led(const byte pin) {
    this->pin = pin;
    pinMode(this->pin, OUTPUT);
  }

  inline void setOn() {
    value = false;
    digitalWrite(pin, HIGH);
  }

  inline void setOff() {
    value = true;
    digitalWrite(pin, LOW);
  }

  bool toggle() {
    if (value) {
      setOff();
    } else {
      setOn();
    }
    return value;
  }
};

class LedTask : public Led, public Task {
 public:
  explicit LedTask(const byte pin) : Led(pin) {}
};
}  // namespace SysLib

#endif