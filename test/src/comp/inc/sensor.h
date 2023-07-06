// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef SENSOR_H
#define SENSOR_H

#include "control.h"
#include "lib/debug.h"
#include "lib/task.h"

namespace SysLib {
class Sensor : public Control {
 private:
  byte pin = 0;
  byte res = 0;
  int currVal = 0;
  int prevVal = 0;
  int valHist[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  byte currIdx = 0;
  void (*onValChangedCB)(const unsigned int) = nullptr;

 public:
  explicit Sensor(const byte pin, const byte res = ANALOG_MAX_BITS) {
    assert(res <= ANALOG_MAX_BITS);
    this->pin = pin;
    this->res = res;
    pinMode(this->pin, INPUT);
  }

  inline void update() {
    // get new reading
    auto newVal = analogRead(pin) >> (ANALOG_MAX_BITS - res);

    // place it in the history index
    valHist[currIdx] = newVal;
    currIdx++;
    if (currIdx == 10) currIdx = 0;

    // find the average value over history, tossing out the highest and lowest value
    int hi = 0, lo = 1 << ANALOG_MAX_BITS, total = 0;
    for (byte i = 0; i < 10; i++) {
      if (valHist[i] > hi) hi = valHist[i];
      if (valHist[i] < lo) lo = valHist[i];
      total += valHist[i];
    }
    total -= hi;
    total -= lo;
    currVal = total / 8;

    // call the callback if the computed value is different than previously calculated
    if (currVal != prevVal) {
      if (onValChangedCB) onValChangedCB(currVal);
    }
    prevVal = currVal;
  }

  inline bool value() const { return currVal; }

  inline void addOnValChangedCB(void (*cb)(const unsigned int)) { onValChangedCB = cb; }
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
