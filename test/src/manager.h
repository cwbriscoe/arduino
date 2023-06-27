// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef MANAGER_H
#define MANAGER_H

#include "lib/thread.h"
#include "tasks.h"

class Manager {
  const byte pwrBtnPin = 8;
  const byte mode1BtnPin = 9;
  const byte mode2BtnPin = 10;

 private:
  ThreadManager threads;  // pointer to our thread manager

  // task declarations
  ButtonTask pwrBtnTask;
  Mode1BtnTask mode1BtnTask;
  Mode2BtnTask mode2BtnTask;
  DisplayTask displayTask;
  LedBlinkTask ledBlinkTask;
  TaskCount taskCount;

 public:
  Manager() : pwrBtnTask(pwrBtnPin), mode1BtnTask(mode1BtnPin), mode2BtnTask(mode2BtnPin) {}

  void init() {
    // if debugging open serial communication over USB
    enableDebuggingSerialPort();

    // hook up listeners
    pwrBtnTask.addListener(&displayTask);

    // add tasks to the thread manager
    threads.add("pwrbtn", 7, (unsigned long)25 * 1000, &pwrBtnTask);
    threads.add("md1btn", 7, (unsigned long)25 * 1000, &mode1BtnTask);
    threads.add("md2btn", 7, (unsigned long)25 * 1000, &mode2BtnTask);
    threads.add("max7219", 8, (unsigned long)50 * 1000, &displayTask);
    threads.add("blink", 8, (unsigned long)500 * 1000, &ledBlinkTask);
#ifdef DEBUG
    threads.add("counter", 9, (unsigned long)50 * 1000, &taskCount);
#endif

    println(F("setup complete"));
  }

  void run() {
    while (true) { threads.run(); }
  }
};

#endif