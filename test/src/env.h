// Copyright 2023 Christopher Briscoe.  All rights reserved.

#include <Arduino.h>

#ifndef ENV_H
#define ENV_H

const byte MAX_THREADS = 7;            // max number ofr threads/tasks that can be added to the thread manager
const byte ANALOG_MAX_BITS = 10;       // number of bits of data provided in analogRead()
const long SERIAL_BAUD_RATE = 115200;  // baud rate to use for serial communications

#endif