// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef TASKS_H
#define TASKS_H

#include "comp/comp.h"
#include "lib/debug.h"
#include "lib/task.h"

/*****************************************************************************/
/* Blink Task                                                                */
/*****************************************************************************/
class LedBlinkTask : public LedTask {
 public:
  LedBlinkTask() : LedTask(LED_BUILTIN) {}

  void run(const Time&) final {
    toggle();
  }
};

/*****************************************************************************/
/* Power Button Task                                                         */
/*****************************************************************************/
class PwrBtnTask : public ButtonTask {
 public:
  PwrBtnTask(const byte pin) : ButtonTask(pin) {}

  void run(const Time&) final {
    ButtonTask::run();
  }
};

/*****************************************************************************/
/* Mode1 Button Task                                                         */
/*****************************************************************************/
class Mode1BtnTask : public ButtonTask {
 public:
  Mode1BtnTask(const byte pin) : ButtonTask(pin) {}

  void run(const Time&) final {
    ButtonTask::run();
    if (isOn()) println(F("mode1 button on..."));
  }
};

/*****************************************************************************/
/* Mode2 Button Task                                                         */
/*****************************************************************************/
class Mode2BtnTask : public ButtonTask {
 public:
  Mode2BtnTask(const byte pin) : ButtonTask(pin) {}

  void run(const Time&) final {
    ButtonTask::run();
    if (isOn()) println(F("mode2 button on..."));
  }
};

/*****************************************************************************/
/* Count Task                                                                */
/*****************************************************************************/
class TaskCount : public Task {
 private:
  Trigger trigger;

 public:
  TaskCount()
      : trigger((unsigned long)1000 * 1000, true) {}

  void run(const Time& time) final {
    if (trigger.triggered(time)) {
      print(F("taskCount time: "));
      println(time.microseconds());
    }
  }
};

/*****************************************************************************/
/* Max7219 Task                                                              */
/*****************************************************************************/
#define MAX_DEVICES 4
#define PWR_PIN 12
#define CS_PIN 3
#define DATA_PIN 4
#define CLK_PIN 2

class DisplayTask : public MX7219Task {
 private:
  Trigger trigger;
  byte mxi;

 public:
  DisplayTask() : MX7219Task(PWR_PIN, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES), trigger(0, true, false), mxi(0) {
    begin();
    // Set the intensity (brightness) of the display (0-15):
    setIntensity(0);
    // Clear the display:
    clear();
  }

  void onControllerChanged(const unsigned long value) final {
    if (!value) return;
    toggleEnabled();
    if (!isEnabled()) {
      clear();
      update();
    }
  }

  void run(const Time& time) final {
    if (!trigger.triggered(time)) return;
    if (!this->isEnabled()) {
      goto RESET;
    }

    clear(0);
    setChar(COL_SIZE - 1, mxi);

    if (MAX_DEVICES >= 3) {
      char hex[3];

      sprintf(hex, "%02X", mxi);

      clear(1);
      setChar((2 * COL_SIZE) - 1, hex[1]);
      clear(2);
      setChar((3 * COL_SIZE) - 1, hex[0]);
    }

    update();
    mxi++;

  RESET:
    trigger.reset((unsigned long)250 * 1000);
  }
};

#endif