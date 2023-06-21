#ifndef DEBUG_H
#define DEBUG_H

// collection of debug only functions

#include "env.h"

#ifdef DEBUG

#define MAX_TIME 0x0fffffff

// miniMicros() overflows every ~70 / 16 minutes to find overflow bugs faster
unsigned long miniMicros() {
  return micros() & MAX_TIME;
}

#define __ASSERT_USE_STDERR
#include <assert.h>

// handle diagnostic informations given by assertion and abort program execution
void __assert(const char *__func, const char *__file, int __lineno, const char *__sexp) {
  // transmit diagnostic informations through serial link
  Serial.print(__func);
  Serial.println(__file);
  Serial.println(__lineno, DEC);
  Serial.println(__sexp);
  Serial.flush();
  // abort program execution
  abort();
}

#else

#define MAX_TIME 0xffffffff

void assert(bool) {} // NOOP for non-debug compile

#endif
#endif