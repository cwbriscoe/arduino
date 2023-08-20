// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef TASKS_H
#define TASKS_H

#include "comp/inc/comp.h"
#include "lib/debug.h"
#include "lib/task.h"

namespace App {
/*****************************************************************************/
/* Blink Task                                                                */
/*****************************************************************************/
class LedBlinkTask : public SysLib::LedTask {
 public:
  LedBlinkTask() : LedTask(LED_BUILTIN) {}

  void run(const SysLib::Time&) final {
    toggle();
  }
};

/*****************************************************************************/
/* Count Task                                                                */
/*****************************************************************************/
class TaskCount : public SysLib::Task {
 private:
  SysLib::Trigger trigger;

 public:
  TaskCount()
      : trigger((unsigned long)1000 * 1000, true) {}

  void run(const SysLib::Time& time) final {
    if (trigger.triggered(time)) {
      print(F("taskCount time: "));
      println(time.microseconds());
    }
  }
};
}  // namespace App

#endif