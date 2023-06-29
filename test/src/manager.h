// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef MANAGER_H
#define MANAGER_H

#include "lib/thread.h"
#include "tasks.h"

class Manager;
static Manager* self;  // pointer to Manager instance to be used in static fuction callbacks

class Manager {
  const byte pwrBtnPin = 8;
  const byte mode1BtnPin = 9;
  const byte mode2BtnPin = 10;
  const byte dialPin = A3;

 private:
  ThreadManager threads;  // pointer to our thread manager

  // task declarations
  ButtonTask pwrBtnTask;
  ButtonTask mode1BtnTask;
  ButtonTask mode2BtnTask;
  DialTask dialTask;
  DisplayTask displayTask;
  LedBlinkTask ledBlinkTask;
  TaskCount taskCount;

 public:
  Manager() : pwrBtnTask(pwrBtnPin), mode1BtnTask(mode1BtnPin), mode2BtnTask(mode2BtnPin), dialTask(dialPin, 4) {
    self = this;
  }

  void init() {
    // add tasks to the thread manager
    threads.add("pwrbtn", 7, (unsigned long)25 * 1000, &pwrBtnTask);
    threads.add("md1btn", 7, (unsigned long)25 * 1000, &mode1BtnTask);
    threads.add("md2btn", 7, (unsigned long)25 * 1000, &mode2BtnTask);
    threads.add("dial", 7, (unsigned long)25 * 1000, &dialTask);
    threads.add("max7219", 8, (unsigned long)50 * 1000, &displayTask);
    threads.add("blink", 8, (unsigned long)500 * 1000, &ledBlinkTask);
#ifdef DEBUG
    threads.add("counter", 9, (unsigned long)50 * 1000, &taskCount);
#endif

    // hook up callbacks
    pwrBtnTask.addOnBtnDownCB(onPwrBtnDown);
    pwrBtnTask.addOnBtnUpCB(onPwrBtnUp);
    mode1BtnTask.addOnBtnDownCB(onMode1BtnDown);
    mode2BtnTask.addOnBtnDownCB(onMode2BtnDown);
    dialTask.addOnValChangedCB(onDialChanged);

    println(F("setup complete"));
  }

  static void onPwrBtnDown() {
    self->displayTask.toggleEnabled();
    if (!self->displayTask.isEnabled()) {
      self->displayTask.clear();
      self->displayTask.update();
    }
  }

  static void onPwrBtnUp() {
    println(F("power button up"));
  }

  static void onMode1BtnDown() {
    println(F("mode1 button down"));
    self->displayTask.incMode();
  }

  static void onMode2BtnDown() {
    println(F("mode2 button down"));
    self->displayTask.decMode();
  }

  static void onDialChanged(const unsigned int val) {
    print(F("dial value: "));
    println(val);
    self->displayTask.setIntensity(val);
  }

  void run() {
    while (true) { threads.run(); }
  }
};

#endif