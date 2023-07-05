// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef RANDOM_H
#define RANDOM_H

#include <EEPROM.h>

#include "debug.h"

namespace SysLib {
unsigned long initRandomSeed() {
  const unsigned long prime = 181081;
  unsigned int seedAddress = 0;

  auto len = EEPROM.length();
  assert(len > 4);
  if (len > 4) seedAddress = len - 4;

  union {
    unsigned long seed;
    byte bytes[4];
  } raw;

  for (byte i = 0; i < sizeof(raw.bytes); i++) {
    raw.bytes[i] = EEPROM.read(seedAddress + i);
  }

  raw.seed += prime;
  unsigned long result = 0;
  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 4; j++) {
      auto val = analogRead(j);
      if (val != 0 && val != (1 << ANALOG_MAX_BITS)) {
        result = (result << 1) | (val & 1);
        delay(1);
      }
    }
  }

  raw.seed ^= result;
  randomSeed(raw.seed);

  for (byte i = 0; i < sizeof(raw.bytes); i++) {
    EEPROM.write(seedAddress + i, raw.bytes[i]);
  }

  return raw.seed;
}
}  // namespace SysLib

#endif