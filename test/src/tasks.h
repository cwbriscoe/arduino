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
/* TMP36 Read Task                                                           */
/*****************************************************************************/
class TMP36ReadTask : public SysLib::TMP36Task {
 public:
  float prevVal = 9999;
  TMP36ReadTask(const byte pin) : TMP36Task(pin) {}

  void run(const SysLib::Time& time) final {
    SensorTask::run(time);
    float val = floatValue();
    if (val != prevVal) {
      float volts = float(val) * 5.0f / 1024.0f;
      float tempC = (volts - 0.5f) * 100;
      float tempF = (tempC * 9) / 5 + 32;
      prevVal = val;
      print(F("current temp = "));
      println(tempF);
    }
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

/*****************************************************************************/
/* Max7219 Task                                                              */
/*****************************************************************************/
#define MAX_DEVICES 4
#define PWR_PIN 12
#define CS_PIN 3
#define DATA_PIN 4
#define CLK_PIN 2

struct Coord {
  byte x;
  byte y;
};

class DisplayTask : public SysLib::MX7219Task {
  const unsigned long modeInterval = (unsigned long)7 * 1000 * 1000;
  const byte modeCount = 10;

 private:
  SysLib::Trigger trigger;
  SysLib::Trigger modeTrigger;
  byte mode = 0;
  byte mxi = 0;

 public:
  DisplayTask() : MX7219Task(PWR_PIN, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES),
                  trigger(0, true, false),
                  modeTrigger(0, true, false) {
  }

  void run(const SysLib::Time& time) final {
    if (!trigger.triggered(time)) return;
    if (modeTrigger.triggered(time)) {
      modeTrigger.reset(modeInterval);
      incMode();
    }
    if (this->isDisabled()) goto RESET;

    switch (mode) {
      case 1:
        demoRowFull();
        break;
      case 2:
        demoSplitRow();
        break;
      case 3:
        demoColumn();
        break;
      case 4:
        demoSplitColumn();
        break;
      case 5:
        demoPoints();
        break;
      case 6:
        demoBounce();
        break;
      case 7:
        demoWorms();
        break;
      case 8:
        demoSawtooth();
        break;
      case 9:
        demoSineWave();
        break;
      case 10:
        demoFont();
        break;
      case 11:
        demoTexter();
        break;
      default:
        clear();
        break;
    }

    update();

  RESET:
    // trigger.reset((unsigned long)150 * 1000);
    trigger.reset((unsigned long)40);
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

  void demoRowFull() {
    setRow(mxi, 0);
    mxi++;
    if (mxi > 7) mxi = 0;
    setRow(mxi, 255);
  }

  void demoSplitRow() {
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

  void demoColumn() {
    setCol(mxi, 0);
    mxi++;
    if (mxi > 7) mxi = 0;
    setCol(mxi, 255);
  }

  void demoSplitColumn() {
    auto mxj = 7 - mxi;
    setCol(mxi, 0x00);
    setCol(mxj, 0x00);
    mxi++;
    if (mxi > 7) mxi = 0;
    mxj = 7 - mxi;
    setCol(mxi, 0x0f);
    setCol(mxj, 0xf0);
  }

  void demoPoints() {
    for (auto dev = 0; dev < MAX_DEVICES; dev++) {
      for (auto i = 0; i < 5; i++) {
        auto row = random(0, 8);
        auto col = random(0, 8);
        setPoint(dev, row, col, false);
      }
      for (auto i = 0; i < 5; i++) {
        auto row = random(0, 8);
        auto col = random(0, 8);
        setPoint(dev, row, col, true);
      }
    }
  }

  void demoBounce() {
    struct Dot {
      Coord coord = {(byte)random(1, 30), (byte)random(1, 7)};
      bool xDir = random(0, 2);
      bool yDir = random(0, 2);
    };
    static Dot dots[8];

    for (auto i = 0; i < 8; i++) {
      if (dots[i].coord.x == 0 || dots[i].coord.x == 31) {
        dots[i].xDir = !dots[i].xDir;
      }
      if (dots[i].coord.y == 0 || dots[i].coord.y == 7) {
        dots[i].yDir = !dots[i].yDir;
      }
      setPointXY(dots[i].coord.x, dots[i].coord.y, false);
      byte spd = 1;
      if (random(0, 100) > 80) {
        if (dots[i].coord.x > 1 && dots[i].coord.x < 30 && dots[i].coord.y > 1 && dots[i].coord.y < 6)
          spd = 2;
      }
      if (dots[i].xDir) {
        dots[i].coord.x += spd;
      } else {
        dots[i].coord.x -= spd;
      }
      if (dots[i].yDir) {
        dots[i].coord.y += spd;
      } else {
        dots[i].coord.y -= spd;
      }
      setPointXY(dots[i].coord.x, dots[i].coord.y, true);
      for (auto j = 0; j < 8; j++) {
        if (i == j) continue;
        if (dots[i].coord.x == dots[j].coord.x && dots[i].coord.y == dots[j].coord.y) {
          if (dots[i].coord.x > 1 && dots[i].coord.x < 30 && dots[i].coord.y > 1 && dots[i].coord.y < 6) {
            if (random(0, 100) > 50) {
              dots[i].xDir = !dots[i].xDir;
            } else {
              dots[i].yDir = !dots[i].yDir;
            }
          }
        }
      }
    }
  }

  void demoWorms() {
    struct Worm {
      Coord coord[5] = {{(byte)random(0, 31), (byte)random(0, 8)}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};
      byte size = 1;
      bool xAxis = random(0, 2);
      bool dir = random(0, 2);
    };
    static Worm worms[5];

    clear();

    for (auto i = 0; i < 5; i++) {
      worms[i].size++;
      if (worms[i].size > 5) worms[i].size = 5;

      for (auto j = (worms[i].size - 2); j >= 0; j--) {
        worms[i].coord[j + 1].x = worms[i].coord[j].x;
        worms[i].coord[j + 1].y = worms[i].coord[j].y;
      }

      auto prevAxis = worms[i].xAxis;
      if (worms[i].xAxis) {
        if (worms[i].coord[1].x == 31 || worms[i].coord[1].x == 0) {
          worms[i].xAxis = false;
          worms[i].dir = random(0, 2);
          if ((worms[i].dir && worms[i].coord[1].y == 7) || (!worms[i].dir && worms[i].coord[1].y == 0)) {
            worms[i].dir = !worms[i].dir;
          }
        }
      } else {
        if (worms[i].coord[1].y == 7 || worms[i].coord[1].y == 0) {
          worms[i].xAxis = true;
          worms[i].dir = random(0, 2);
          if ((worms[i].dir && worms[i].coord[1].x == 31) || (!worms[i].dir && worms[i].coord[1].x == 0)) {
            worms[i].dir = !worms[i].dir;
          }
        }
      }

      if ((prevAxis == worms[i].xAxis) && random(0, 100) > 85) {
        if (worms[i].xAxis) {
          worms[i].xAxis = false;
          worms[i].dir = random(0, 2);
          if ((worms[i].dir && worms[i].coord[1].y == 7) || (!worms[i].dir && worms[i].coord[1].y == 0)) {
            worms[i].dir = !worms[i].dir;
          }
        } else {
          worms[i].xAxis = true;
          worms[i].dir = random(0, 2);
          if ((worms[i].dir && worms[i].coord[1].x == 31) || (!worms[i].dir && worms[i].coord[1].x == 0)) {
            worms[i].dir = !worms[i].dir;
          }
        }
      }

      if (worms[i].xAxis) {
        if (worms[i].dir) {
          worms[i].coord[0].x = worms[i].coord[1].x + 1;
        } else {
          worms[i].coord[0].x = worms[i].coord[1].x - 1;
        }
      } else {
        if (worms[i].dir) {
          worms[i].coord[0].y = worms[i].coord[1].y + 1;
        } else {
          worms[i].coord[0].y = worms[i].coord[1].y - 1;
        }
      }

      for (auto j = 0; j < worms[i].size; j++) {
        setPointXY(worms[i].coord[j].x, worms[i].coord[j].y, true);
      }
    }
  }

  void demoSawtooth() {
    static byte prevLine = 0;
    static bool prevUpDir = true;

    byte line = prevLine;
    bool upDir = prevUpDir;
    if (upDir) {
      if (line < 7) {
        line++;
      } else {
        line--;
        upDir = false;
      }
    } else {
      if (line > 0) {
        line--;
      } else {
        line++;
        upDir = true;
      }
    }
    prevLine = line;
    prevUpDir = upDir;

    byte lineCount = 0;
    clear();

    for (auto x = 0; x < 32; x++) {
      setPointXY(x, line, true);
      lineCount++;

      if (lineCount == 2) {
        lineCount = 0;
        if (upDir) {
          if (line < 7) {
            line++;
          } else {
            line--;
            upDir = false;
          }
        } else {
          if (line > 0) {
            line--;
          } else {
            line++;
            upDir = true;
          }
        }
      }
    }
  }

  void demoSineWave() {
    static float radians = 0.0f;
    static float incDivisor = 10.0f;
    static float inc = 2 * PI / incDivisor;

    clear();
    auto currRadians = radians;
    byte prevY = 255;

    for (byte x = 0; x < 32; x++) {
      byte y = sin(currRadians) * 4 + 4;
      if (y > 7) y = 7;
      if (prevY < 255 && prevY < y) {
        for (auto i = prevY + 1; i <= y; i++) {
          setPointXY(x, i, true);
        }
      } else if (prevY < 255 && prevY > y) {
        for (auto i = prevY - 1; i >= y; i--) {
          setPointXY(x, i, true);
        }
      } else {
        setPointXY(x, y, true);
      }
      currRadians += inc;
      prevY = y;
    }

    radians += inc;
    if (radians > 2 * PI) radians -= 2 * PI;
  }

  void demoFont() {
    const byte maxDigits = 65;
    static byte currDigit = 0;

    currDigit++;
    if (currDigit == maxDigits) currDigit = 0;

    auto idx = currDigit;
    setDigit(3, idx);
    idx++;
    if (idx == maxDigits) idx = 0;
    setDigit(2, idx);
    idx++;
    if (idx == maxDigits) idx = 0;
    setDigit(1, idx);
    idx++;
    if (idx == maxDigits) idx = 0;
    setDigit(0, idx);
  }

  void demoTexter() {
    static auto texter = new SysLib::Texter(this, "Chris");
    texter->write();
  }
};
}  // namespace App

#endif