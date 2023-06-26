// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef MANAGER_H
#define MANAGER_H

#include "lib/thread.h"
#include "tasks.h"

class Manager {
 private:
  ThreadManager threads;  // pointer to our thread manager
  // task declarations
  TaskMax7219 taskMax7219;
  TaskBlink taskBlink;
  TaskButtons taskButtons;
  TaskCount taskCount;

 public:
  Manager() {}

  void init() {
    // if debugging open serial communication over USB
#ifdef DEBUG
    Serial.begin(115200);
    // delay a bit in debug mode so serial output isn't garbled on first Serial.print()
    while (!Serial) {}
    delay(100);
#endif

    // initialize the thread manager and worker threads
    threads.add("buttons", 7, (unsigned long)25 * 1000, &taskButtons);
    taskButtons.init();

    threads.add("max7219", 8, (unsigned long)50 * 1000, &taskMax7219);
    taskMax7219.init();

    threads.add("blink", 8, (unsigned long)500 * 1000, &taskBlink);
    taskBlink.init();

#ifdef DEBUG
    threads.add("counter", 9, (unsigned long)50 * 1000, &taskCount);
    taskCount.init();
#endif

    println(F("setup complete"));
  }

  void run() {
    while (true) { threads.run(); }
  }
};

#endif