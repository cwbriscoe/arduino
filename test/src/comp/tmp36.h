// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef TMP36_H
#define TMP36_H

#include "inc/sensor.h"

namespace SysLib {
class TMP36 : public Sensor {
 private:
  float prevVal = 0.0f;
  void (*onFloatValChangedCB)(const float) = nullptr;

  void update() {
    float val = floatValue();
    if (val != prevVal) {
      prevVal = val;
      if (onFloatValChangedCB) onFloatValChangedCB(val);
    }
  }

  inline void addOnFloatValChangedCB(void (*cb)(const float)) { onFloatValChangedCB = cb; }

 public:
  explicit TMP36(const byte pin, const byte res = ANALOG_MAX_BITS) : Sensor(pin, res) {}
};

class TMP36Task : public SensorTask {
 public:
  explicit TMP36Task(const byte pin, const byte res = ANALOG_MAX_BITS) : SensorTask(pin, res) {}

  void run(const Time&) override {
    Sensor::update();
    update();
  }
};
}  // namespace SysLib

#endif