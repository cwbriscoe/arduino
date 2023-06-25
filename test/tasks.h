// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef TASKS_H
#define TASKS_H

#include "debug.h"
#include "led.h"
#include "mx7219.h"

/*****************************************************************************/
/* Blink Task                                                                */
/*****************************************************************************/
class TaskBlink : public Task {
private:
  bool led = false;

public:
  void init() final {
    setupLED(LED_BUILTIN);
  }

  void run(const Time&) final {
    if (led) {
      turnOnLED(LED_BUILTIN);
    } else {
      turnOffLED(LED_BUILTIN);
    }

    led = !led;
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

  void init() final {}

  void run(const Time& time) final {
    if (trigger.triggered(time)) {
      Serial.print("taskCount time: ");
      Serial.println(time.microseconds());
    }
  }
};

/*****************************************************************************/
/* Max7219 Task                                                              */
/*****************************************************************************/
#define MAX_DEVICES 4
#define CS_PIN 3
#define DATA_PIN 2
#define CLK_PIN 4

class TaskMax7219 : public Task {
private:
  Trigger trigger;
  MX7219 mx;
  byte mxi;

public:
  TaskMax7219()
    : trigger(0, true, false), mx(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES), mxi(0) {}

  void init() final {
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