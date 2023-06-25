// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef LED_H
#define LED_H

class Led {
 private:
  byte pin = 0;
  bool value = false;

 public:
  Led(const byte pin) {
    this->pin = pin;
    pinMode(this->pin, OUTPUT);
  }

  void setOn() {
    digitalWrite(pin, HIGH);
  }

  void setOff() {
    digitalWrite(pin, LOW);
  }

  bool toggle() {
    value = !value;
    if (value) {
      setOn();
    } else {
      setOff();
    }
    return value;
  }
};

#endif