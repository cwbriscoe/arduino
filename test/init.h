#ifndef INIT_H
#define INIT_H

#include "env.h"
#include "thread.h"
#include "tasks.h"

ThreadManager threadManager;

void initialize() {
  // if debugging open serial communication over USB
#ifdef DEBUG
  Serial.begin(9600);
#endif

  // initialize the thread manager and worker threads
  threadManager.add("blink", 1, (unsigned long)(200) * 1000, task1);
}

#endif