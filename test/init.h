#ifndef INIT_H
#define INIT_H

#include "thread.h"
#include "tasks.h"

ThreadManager threadManager;

void initialize() {
  // if debugging open serial communication over USB
#ifdef DEBUG
  Serial.begin(9600);
  // delay a bit in debug mode so serial output isn't garbled on first Serial.print()
  while (!Serial) {}
  delay(100);
#endif

  // initialize the thread manager and worker threads
  threadManager.add("blink", 1, (unsigned long)500 * 1000, taskBlink);
#ifdef DEBUG
  threadManager.add("counter", 3, (unsigned long)1, taskCount);
#endif

  // setup pin configs
  setupLED(LED_BUILTIN);
}

#endif