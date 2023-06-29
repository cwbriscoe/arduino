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
  const unsigned long modeInterval = (unsigned long)15 * 1000 * 1000;
  const byte modeCount = 2;

 private:
  Trigger trigger;
  Trigger modeTrigger;
  byte mode = 0;
  byte mxi = 0;

 public:
  DisplayTask() : MX7219Task(PWR_PIN, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES),
                  trigger(0, true, false),
                  modeTrigger(0, true, false) {
  }

  void run(const Time& time) final {
    if (!trigger.triggered(time)) return;
    if (modeTrigger.triggered(time)) {
      modeTrigger.reset(modeInterval);
      incMode();
    }
    if (!this->isEnabled()) goto RESET;

    switch (mode) {
      case 1:
        rowFull();
        break;
      case 2:
        splitRow();
        break;
      default:
        clear();
        break;
    }

    update();

  RESET:
    trigger.reset((unsigned long)250 * 1000);
  }

  void incMode() {
    mxi = 0;
    clear();
    mode++;
    if (mode > modeCount) mode = 1;
  }

  void decMode() {
    mxi = 0;
    clear();
    mode--;
    if (mode == 0) mode = modeCount;
  }

  void rowFull() {
    setRow(mxi, 0);
    mxi++;
    if (mxi > 7) mxi = 0;
    setRow(mxi, 255);
  }

  void splitRow() {
    int a = mxi, b = mxi + 2, c = mxi + 4, d = mxi + 6;
    if (b > 7) b -= 8;
    if (c > 7) c -= 8;
    if (d > 7) d -= 8;
    setRow(0, a, 0);
    setRow(1, b, 0);
    setRow(2, c, 0);
    setRow(3, d, 0);
    mxi++;
    if (mxi > 7) mxi = 0;
    a = mxi;
    b = mxi + 2;
    c = mxi + 4;
    d = mxi + 6;
    if (b > 7) b -= 8;
    if (c > 7) c -= 8;
    if (d > 7) d -= 8;
    setRow(0, a, 255);
    setRow(1, b, 255);
    setRow(2, c, 255);
    setRow(3, d, 255);
  }
};

#endif