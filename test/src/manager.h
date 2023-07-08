// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef MANAGER_H
#define MANAGER_H

#include "lib/random.h"
#include "lib/thread.h"
#include "tasks.h"

namespace App {
const char pwrbtnNm[] PROGMEM = "pwrbtn";
const char md1btnNm[] PROGMEM = "md1btn";
const char md2btnNm[] PROGMEM = "md2btn";
const char dialNm[] PROGMEM = "dial";
const char tmp36Nm[] PROGMEM = "tmp36";
const char max7219Nm[] PROGMEM = "max7219";
const char blinkNm[] PROGMEM = "blink";
#ifdef DEBUG
const char counterNm[] PROGMEM = "counter";
#endif

class Manager;
static Manager* self;  // pointer to Manager instance to be used in static fuction callbacks

class Manager {
  const byte pwrBtnPin = 8;
  const byte mode1BtnPin = 9;
  const byte mode2BtnPin = 10;
  const byte dialPin = A3;
  const byte tmp36Pin = A2;

 private:
  SysLib::ThreadManager threads;  // pointer to our thread manager

  // task declarations
  SysLib::ButtonTask pwrBtnTask;
  SysLib::ButtonTask mode1BtnTask;
  SysLib::ButtonTask mode2BtnTask;
  SysLib::DialTask dialTask;
  DisplayTask displayTask;
  TMP36ReadTask tmp36ReadTask;
  LedBlinkTask ledBlinkTask;
  TaskCount taskCount;

 public:
  Manager() : pwrBtnTask(pwrBtnPin), mode1BtnTask(mode1BtnPin), mode2BtnTask(mode2BtnPin), dialTask(dialPin, 4), tmp36ReadTask(tmp36Pin) {
    self = this;
  }

  void init() {
    // add tasks to the thread manager
    threads.add(pwrbtnNm, 6, (unsigned long)25 * 1000, &pwrBtnTask);
    threads.add(md1btnNm, 6, (unsigned long)25 * 1000, &mode1BtnTask);
    threads.add(md2btnNm, 6, (unsigned long)25 * 1000, &mode2BtnTask);
    threads.add(dialNm, 7, (unsigned long)25 * 1000, &dialTask);
    threads.add(tmp36Nm, 7, (unsigned long)100 * 1000, &tmp36ReadTask);
    threads.add(max7219Nm, 8, (unsigned long)40 * 1000, &displayTask);
    threads.add(blinkNm, 8, (unsigned long)500 * 1000, &ledBlinkTask);
#ifdef DEBUG
    threads.add(counterNm, 9, (unsigned long)50 * 1000, &taskCount);
#endif

    // hook up callbacks
    pwrBtnTask.addOnBtnDownCB(onPwrBtnDown);
    pwrBtnTask.addOnBtnUpCB(onPwrBtnUp);
    mode1BtnTask.addOnBtnDownCB(onMode1BtnDown);
    mode2BtnTask.addOnBtnDownCB(onMode2BtnDown);
    dialTask.addOnValChangedCB(onDialChanged);

    // comment out initRandomSeed() for deterministic random numbers
    print(F("setup complete : seed = "));
    println(SysLib::initRandomSeed());
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
    self->displayTask.decMode();
  }

  static void onMode2BtnDown() {
    println(F("mode2 button down"));
    self->displayTask.incMode();
  }

  static void onDialChanged(const word val) {
    print(F("dial value: "));
    println(val);
    self->displayTask.setIntensity(val);
  }

  void run() {
    while (true) { threads.run(); }
  }
};
}  // namespace App

#endif