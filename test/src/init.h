// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef INIT_H
#define INIT_H

#include "tasks.h"
#include "thread.h"

ThreadManager threadManager;

// init our code before we go into the endless loop
void initialize() {
  // if debugging open serial communication over USB
#ifdef DEBUG
  Serial.begin(115200);
  // delay a bit in debug mode so serial output isn't garbled on first Serial.print()
  while (!Serial) {}
  delay(100);
#endif

  // initialize the thread manager and worker threads
  threadManager.add("max7219", 8, (unsigned long)10 * 1000, new TaskMax7219());
  threadManager.add("blink", 8, (unsigned long)500 * 1000, new TaskBlink());
#ifdef DEBUG
  threadManager.add("counter", 9, (unsigned long)50 * 1000, new TaskCount());
#endif

  println(F("setup complete"));
}

#endif