// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef TMP36_H
#define TMP36_H

#include "inc/sensor.h"

namespace SysLib {
class TMP36 : public Sensor {
 private:
  float prevVal = 0.0f;
  void (*onCelsiusChangedCB)(const float) = nullptr;
  void (*onFahrenheitChangedCB)(const float) = nullptr;

 public:
  explicit TMP36(const byte pin, const byte res = ANALOG_MAX_BITS) : Sensor(pin, res) {}

  void update() {
    float val = floatValue();
    if (val != prevVal) {
      prevVal = val;
      if (onCelsiusChangedCB) onCelsiusChangedCB(celsius());
      if (onFahrenheitChangedCB) onFahrenheitChangedCB(fahrenheit());
    }
  }

  inline float celsius() {
      return (((prevVal) * 5.0f / 1024.0f) - 0.5f) * 100;
  }

  inline float fahrenheit() {
      return (celsius() * 9) / 5 + 32;
  }

  inline void addOnCelsiusChangedCB(void (*cb)(const float)) { onCelsiusChangedCB = cb; }
  inline void addOnFahrenheitChangedCB(void (*cb)(const float)) { onFahrenheitChangedCB = cb; }
};

class TMP36Task : public Task, public TMP36 {
 public:
  explicit TMP36Task(const byte pin, const byte res = ANALOG_MAX_BITS) : TMP36(pin, res) {}

  void run(const Time&) override {
      Sensor::update();
      update();
  }
};
}  // namespace SysLib

#endif