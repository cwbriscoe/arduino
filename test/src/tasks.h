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
  }

  void run(const Time& time) final {
    if (!trigger.triggered(time)) return;
    if (!this->isEnabled()) {
      goto RESET;
    }

    goto TEST;

    clear(0);
    setChar(MX_COL_SIZE - 1, mxi);

    if (MAX_DEVICES >= 3) {
      char hex[3];

      sprintf(hex, "%02X", mxi);

      clear(1);
      setChar((2 * MX_COL_SIZE) - 1, hex[1]);
      clear(2);
      setChar((3 * MX_COL_SIZE) - 1, hex[0]);
    }

  TEST:
    setRow(mxi, 0);
    mxi++;
    if (mxi > 7) mxi = 0;
    setRow(mxi, 255);

    update();

  RESET:
    trigger.reset((unsigned long)250 * 1000);
  }
};

#endif