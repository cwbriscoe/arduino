// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef TASKS_H
#define TASKS_H

#include "comp/comp.h"
#include "lib/debug.h"
#include "lib/task.h"

/*****************************************************************************/
/* Blink Task                                                                */
/*****************************************************************************/
class TaskBlink : public Task {
 private:
  Led led;

 public:
  TaskBlink() : led(LED_BUILTIN) {}

  void init() {}

  void run(const Time&) final {
    led.toggle();
  }
};

/*****************************************************************************/
/* Buttons Task                                                              */
/*****************************************************************************/
class TaskButtons : public Task {
 private:
  const byte pwrBtnPin = 8;
  const byte mode1BtnPin = 9;
  const byte mode2BtnPin = 10;
  Button pwrBtn;
  Button mode1Btn;
  Button mode2Btn;

 public:
  TaskButtons() : pwrBtn(pwrBtnPin), mode1Btn(mode1BtnPin), mode2Btn(mode2BtnPin) {}

  void init() {}

  void run(const Time&) final {
    if (pwrBtn.isOn()) {
      println("power is on...");
    }
    if (mode1Btn.isOn()) {
      println("mode1 is on...");
    }
    if (mode2Btn.isOn()) {
      println("mode2 is on...");
    }
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

  void init() {}

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

class TaskMax7219 : public Task {
 private:
  Trigger trigger;
  MX7219 mx;
  byte mxi;

 public:
  TaskMax7219()
      : trigger(0, true, false), mx(PWR_PIN, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES), mxi(0) {}

  void init() {
    mx.begin();
    // Set the intensity (brightness) of the display (0-15):
    mx.setIntensity(0);
    // Clear the display:
    mx.clear();
  }

  void run(const Time& time) final {
    if (!trigger.triggered(time)) { return; }

    mx.clear(0);
    mx.setChar(COL_SIZE - 1, mxi);

    if (MAX_DEVICES >= 3) {
      char hex[3];

      sprintf(hex, "%02X", mxi);

      mx.clear(1);
      mx.setChar((2 * COL_SIZE) - 1, hex[1]);
      mx.clear(2);
      mx.setChar((3 * COL_SIZE) - 1, hex[0]);
    }

    mx.update();
    mxi++;
    trigger.reset((unsigned long)250 * 1000);
  }
};

#endif