// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef DEBUG_H
#define DEBUG_H

// collection of debug utility functions

#include "env.h"

// cause time to overflow faster in debug mode to catch errors more quickly 
// this will cause it to overflow about every 16 seconds
#ifdef DEBUG
#define MAX_TIME 0x00ffffff
#else
#define MAX_TIME 0xffffffff
#endif

// miniMicros() overflows every ~70 minutes / 16 seconds to find overflow bugs faster
#ifdef DEBUG
unsigned long miniMicros() {
  return micros() & MAX_TIME;
}
#endif

// handle diagnostic informations given by assertion and abort program execution
#ifdef DEBUG
#define __ASSERT_USE_STDERR
#include <assert.h>
void __assert(const char *__func, const char *__file, int __lineno, const char *__sexp) {
  // transmit diagnostic informations through serial link
  Serial.println();
  Serial.println("ASSERTION FAILED!");
  Serial.println(__func);
  Serial.println(__file);
  Serial.println(__lineno, DEC);
  Serial.println(__sexp);
  Serial.flush();
  // abort program execution
  abort();
}
#else
void assert(bool) {}  // NOOP for non-debug compile
#endif

#endif