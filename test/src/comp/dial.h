// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef DIAL_H
#define DIAL_H

#include "inc/sensor.h"

namespace SysLib {
class Dial : public Sensor {
 public:
  explicit Dial(const byte pin, const byte res = ANALOG_MAX_BITS) : Sensor(pin, res) {}
};

class DialTask : public SensorTask {
 public:
  explicit DialTask(const byte pin, const byte res = ANALOG_MAX_BITS) : SensorTask(pin, res) {}
};
}  // namespace SysLib

#endif
