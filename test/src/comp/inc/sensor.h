// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef SENSOR_H
#define SENSOR_H

#include "control.h"
#include "lib/debug.h"
#include "lib/task.h"

namespace SysLib {
class Sensor : public Control {
  const byte numSamples = 10;

 private:
  byte pin = 0;
  byte res = 0;
  word total = 0;
  word currVal = 0;
  word prevVal = 0;
  void (*onValChangedCB)(const word) = nullptr;

 public:
  explicit Sensor(const byte pin, const byte res = ANALOG_MAX_BITS) {
    assert(res <= ANALOG_MAX_BITS);
    this->pin = pin;
    this->res = res;
    pinMode(this->pin, INPUT);
    for (byte i = 0; i < numSamples; i++) {
      total += analogRead(this->pin);
    }
  }

  void update() {
    // get new reading
    int val = analogRead(pin) >> (ANALOG_MAX_BITS - res);
    total -= (total / numSamples);
    total += val;
    currVal = total / numSamples;

    // call the callback if the computed value is different than previously calculated
    if (currVal != prevVal) {
      prevVal = currVal;
      if (onValChangedCB) onValChangedCB(currVal);
    }
  }

  inline word value() const { return currVal; }
  inline float const floatValue() { return float(total) / float(numSamples); }

  inline void addOnValChangedCB(void (*cb)(const word)) { onValChangedCB = cb; }
};

class SensorTask : public Sensor, public Task {
 public:
  explicit SensorTask(const byte pin, const byte res = ANALOG_MAX_BITS) : Sensor(pin, res) {}

  void run(const Time&) override {
    update();
  }
};
}  // namespace SysLib

#endif