#ifndef INIT_H
#define INIT_H

#include "thread.h"
#include "tasks.h"

ThreadManager threadManager;

void initialize() {
  // if debugging open serial communication over USB
#ifdef DEBUG
  Serial.begin(9600);
  while (!Serial) {}
#endif

  // initialize the thread manager and worker threads
  //threadManager.add("blink", 1, (unsigned long)500 * 1000, task1);
  //threadManager.add("counter 1", 2, (unsigned long)1000 * 1000, task2);
  //threadManager.add("counter 2", 3, (unsigned long)1 * 1000, task3);
  //threadManager.add("counter 2", 3, (unsigned long)1, task3);
}

#endif