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
  byte currIdx = 0;
  byte currFloatIdx = 0;
  word currVal = 0;
  word prevReading = 0;
  word valHist[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  float valFloatHist[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  void (*onValChangedCB)(const word) = nullptr;

 public:
  explicit Sensor(const byte pin, const byte res = ANALOG_MAX_BITS) {
    assert(res <= ANALOG_MAX_BITS);
    this->pin = pin;
    this->res = res;
    pinMode(this->pin, INPUT);
  }

  void update() {
    // get new reading
    word newReading = analogRead(pin) >> (ANALOG_MAX_BITS - res);
    if (newReading != prevReading && newReading == valHist[currIdx]) {
      prevReading = newReading;
      return;
    }

    // place it in the history index
    valHist[currIdx] = newReading;
    word prevVal = valHist[currIdx];
    currIdx++;
    if (currIdx == 10) currIdx = 0;

    // find the average value over history, tossing out the highest and lowest value
    word hi = 0, lo = 1 << ANALOG_MAX_BITS, total = 0;
    for (byte i = 0; i < 10; i++) {
      if (valHist[i] > hi) hi = valHist[i];
      if (valHist[i] < lo) lo = valHist[i];
      total += valHist[i];
    }
    total -= hi;
    total -= lo;

    valFloatHist[currFloatIdx] = float(total) / 8.0f;
    currVal = (valFloatHist[currFloatIdx] + 0.5f);
    currFloatIdx++;
    if (currFloatIdx == 5) currFloatIdx = 0;

    // call the callback if the computed value is different than previously calculated
    if (currVal != prevVal) {
      if (onValChangedCB) onValChangedCB(currVal);
    }
  }

  float floatValue() const {
    float total = 0.0f;
    for (auto i = 0; i < 5; i++) {
      total += valFloatHist[i];
    }
    return total / 5.0f;
  }

  inline word value() const { return currVal; }

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