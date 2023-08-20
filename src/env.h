// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef ENV_H
#define ENV_H

#include <Arduino.h>

namespace SysLib {
const byte MAX_THREADS = 8;         // max number ofr threads/tasks that can be added to the thread manager
const byte ANALOG_MAX_BITS = 10;    // number of bits of data provided in analogRead()
const byte SRL_INIT_DELAY = 100;    // time in milliseconds to delay after starting serial communications
const long SRL_BAUD_RATE = 115200;  // baud rate to use for serial communications
}  // namespace SysLib

#endif