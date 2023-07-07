// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef TMP36_H
#define TMP36_H

#include "inc/sensor.h"

namespace SysLib {
class TMP36 : public Sensor {
 public:
  explicit TMP36(const byte pin, const byte res = ANALOG_MAX_BITS) : Sensor(pin, res) {}
};

class TMP36Task : public SensorTask {
 public:
  explicit TMP36Task(const byte pin, const byte res = ANALOG_MAX_BITS) : SensorTask(pin, res) {}
};
}  // namespace SysLib

#endif