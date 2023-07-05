// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef mx7219_H
#define mx7219_H

#include "inc/control.h"
#include "inc/font.h"

// control registers (1-8 = digits 0-7)
#define MX_NOOP 0
#define MX_DIGIT0 1
#define MX_DECODEMODE 9
#define MX_INTENSITY 10
#define MX_SCANLIMIT 11
#define MX_SHUTDOWN 12
#define MX_DISPLAYTEST 15

// other constants
#define MX_ROW_SIZE 8
#define MX_COL_SIZE 8

namespace SysLib {
class MX7219 : public Control {
 private:
  struct DeviceInfo {
    byte row[MX_ROW_SIZE];  // data for each row 0=OFF 1=ON
  };

  byte pwrPin;             // pin used to supply power
  byte dataPin;            // output where data is sent
  byte clkPin;             // output for the clock signal
  byte csPin;              // output for selecting which device to write to
  byte devices;            // number of devices
  byte intensity;          // brightness of the display
  DeviceInfo* currMatrix;  // stores the LED matrix buffers for the current frame
  DeviceInfo* prevMatrix;  // stores the LED matrix buffers for the previous frame
  byte* rowCounts;         // keep track of which rows for each device that have been sent
  byte* busData;           // data buffer for writing to the data bus

  inline byte busDataSize() const {
    return sizeof(byte) * devices * 2;
  }

  inline byte busOffset(const byte dev, const byte digit) const {
    return ((devices - 1) - dev) * 2 + digit;
  }

  inline void clearBusBuffer() const {
    memset(busData, MX_NOOP, busDataSize());
  }

  void controlHardware(const byte dev, const byte mode, const int val) const {
    byte opcode = MX_NOOP;
    byte param = 0;

    // work out data to write
    switch (mode) {
      case MX_SHUTDOWN:
        opcode = MX_SHUTDOWN;
        param = (val == 0 ? 1 : 0);
        break;
      case MX_SCANLIMIT:
        opcode = MX_SCANLIMIT;
        param = (val > 7 ? 7 : val);
        break;
      case MX_INTENSITY:
        opcode = MX_INTENSITY;
        param = (val > 15 ? 15 : val);
        break;
      case MX_DECODEMODE:
        opcode = MX_DECODEMODE;
        param = (val == 0 ? 0 : 0xff);
        break;
      case MX_DISPLAYTEST:
        opcode = MX_DISPLAYTEST;
        param = (val == 0 ? 0 : 1);
        break;
      default:
        return;
    }

    // put our device data into the buffer
    busData[busOffset(dev, 0)] = opcode;
    busData[busOffset(dev, 1)] = param;
  }

  void busSend() const {
    digitalWrite(csPin, LOW);

    auto sz = busDataSize();

    for (auto i = 0; i < sz; i++) {
      shiftOut(dataPin, clkPin, MSBFIRST, busData[i]);
    }

    digitalWrite(csPin, HIGH);
  }

  void flush() const {
    memset(rowCounts, 0, sizeof(byte) * devices);

    bool changed;
    do {
      changed = false;
      clearBusBuffer();
      for (byte dev = 0; dev < devices; dev++) {
        while ((rowCounts[dev] < MX_ROW_SIZE)) {
          if (currMatrix[dev].row[rowCounts[dev]] != prevMatrix[dev].row[rowCounts[dev]]) {
            busData[busOffset(dev, 0)] = MX_DIGIT0 + rowCounts[dev];
            busData[busOffset(dev, 1)] = currMatrix[dev].row[rowCounts[dev]];
            rowCounts[dev]++;
            changed = true;
            break;
          }
          rowCounts[dev]++;
        }
      }
      if (changed) busSend();
    } while (changed);

    for (byte dev = 0; dev < devices; dev++)
      memcpy(&prevMatrix[dev], &currMatrix[dev], sizeof(currMatrix[dev]));
  }

 public:
  MX7219(const byte pwrPin, const byte dataPin, const byte clkPin, const byte csPin, const byte devices = 1) {
    this->pwrPin = pwrPin;
    this->dataPin = dataPin;
    this->clkPin = clkPin;
    this->csPin = csPin;
    this->devices = devices;
    this->intensity = 0;

    // allocate memory needed for buffers anc clear them
    this->currMatrix = new DeviceInfo[this->devices];
    this->prevMatrix = new DeviceInfo[this->devices];
    this->rowCounts = new byte[this->devices];
    this->busData = new byte[this->busDataSize()];
    clear();
    clearBusBuffer();
    memset(prevMatrix[this->devices].row, 0, sizeof(prevMatrix[this->devices].row));

    // set all pins to output
    pinMode(this->pwrPin, OUTPUT);
    pinMode(this->dataPin, OUTPUT);
    pinMode(this->clkPin, OUTPUT);
    pinMode(this->csPin, OUTPUT);
    digitalWrite(this->csPin, HIGH);
    digitalWrite(this->pwrPin, HIGH);

    // initialize driver and turn auto updates off
    control(MX_DISPLAYTEST, 0);              // no test
    control(MX_SCANLIMIT, MX_ROW_SIZE - 1);  // scan limit is set to max on startup
    control(MX_DECODEMODE, 0);               // ensure no decoding (warm boot potential issue)
    control(MX_SHUTDOWN, 0);                 // take the modules out of shutdown mode

    setIntensity(0);  // default to lowest brightness
  }

  ~MX7219() {
    delete this->currMatrix;
    delete this->prevMatrix;
    delete this->rowCounts;
    delete this->busData;
  }

  // forbid copies
  MX7219(const MX7219&) = delete;
  MX7219& operator=(const MX7219&) = delete;

  void setIntensity(const byte intensity) {
    this->intensity = intensity;
    control(MX_INTENSITY, this->intensity);
  }

  void clear(const byte dev) const {
    memset(currMatrix[dev].row, 0, sizeof(currMatrix[dev].row));
  }

  void clear() const {
    for (byte dev = 0; dev < devices; dev++) {
      clear(dev);
    }
  }

  void control(const byte mode, const int val) {
    clearBusBuffer();
    for (auto dev = 0; dev < devices; dev++)
      controlHardware(dev, mode, val);
    busSend();
  }

  void update() const {
    flush();
  }

  void setRow(const byte dev, const byte row, const byte val) const {
    assert(dev < devices);
    assert(row < MX_ROW_SIZE);
    currMatrix[dev].row[row] = val;
  }

  void setRow(const byte row, const byte val) const {
    for (byte dev = 0; dev < devices; dev++) {
      setRow(dev, row, val);
    }
  }

  void setCol(const byte dev, const byte col, const byte val) const {
    assert(dev < devices);
    assert(col < MX_COL_SIZE);
    for (auto row = 0; row < MX_COL_SIZE; row++) {
      if (val & (1 << row)) {
        bitSet(currMatrix[dev].row[row], col);
      } else {
        bitClear(currMatrix[dev].row[row], col);
      }
    }
  }

  void setCol(const byte col, const byte val) const {
    for (byte dev = 0; dev < devices; dev++) {
      setCol(dev, col, val);
    }
  }

  void setPoint(const byte dev, const byte row, const byte col, const bool lit) const {
    assert(dev < devices);
    assert(row < MX_ROW_SIZE);
    assert(col < MX_COL_SIZE);
    if (lit) {
      bitSet(currMatrix[dev].row[row], col);
    } else {
      bitClear(currMatrix[dev].row[row], col);
    }
  }

  void setPoint(const byte row, const byte col, const bool lit) const {
    for (byte dev = 0; dev < devices; dev++) {
      setPoint(dev, row, col, lit);
    }
  }

  void setPointXY(const byte x, const byte y, const bool lit) const {
    byte dev = (devices - 1) - (x >> 3);
    setPoint(dev, 7 - y, 7 - (x & 7), lit);
  }

  byte getChar(const char chr) const {
    if (chr < 32) { return 0; }
    if (chr > 122) { return 0; }
    return chr - 32;
  }

  void setDigit(const byte dev, const byte digit) const {
    for (auto i = 0; i < 7; i++) {
      auto bits = pgm_read_byte(&font[digit][i]);
      setRow(dev, i, bits);
    }
  }

  void setChar(const byte dev, const char chr) const {
    setDigit(dev, getChar(chr));
  }
};

class MX7219Task : public MX7219, public Task {
 public:
  explicit MX7219Task(const byte pwrPin, const byte dataPin, const byte clkPin, const byte csPin, const byte devices = 1)
      : MX7219(pwrPin, dataPin, clkPin, csPin, devices) {}
};
}  // namespace SysLib

#endif