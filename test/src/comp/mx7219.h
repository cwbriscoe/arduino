// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef mx7219_H
#define mx7219_H

#include "control.h"

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

class MX7219 : public Control {
 private:
  struct DeviceInfo {
    byte row[MX_ROW_SIZE];  // data for each row 0=OFF 1=ON
    byte changed;           // each bit is 1 if the row has changed
  };

  byte pwrPin;         // pin used to supply power
  byte dataPin;        // output where data is sent
  byte clkPin;         // output for the clock signal
  byte csPin;          // output for selecting which device to write to
  byte devices;        // number of devices
  byte intensity;      // brightness of the display
  DeviceInfo* matrix;  // stores the LED matrix buffers
  byte* spiData;       // data buffer for writing to the SPI interface

  inline byte spiDataSize() const {
    return sizeof(byte) * devices * 2;
  }

  inline byte spiOffset(const byte dev, const byte digit) {
    return ((devices - 1) - dev) * 2 + digit;
  }

  inline void clearSpiBuffer() {
    memset(spiData, MX_NOOP, spiDataSize());
  }

  void controlHardware(const byte dev, const byte mode, const int val) {
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
    spiData[spiOffset(dev, 0)] = opcode;
    spiData[spiOffset(dev, 1)] = param;
  }

  void spiSend() {
    digitalWrite(csPin, LOW);

    auto sz = spiDataSize();

    for (auto i = 0; i < sz; i++) {
      auto val = spiData[i];
      for (byte j = 0; j < 8; j++) {
        digitalWrite(dataPin, (val & 0x80) != 0);
        val <<= 1;
        digitalWrite(clkPin, HIGH);
        digitalWrite(clkPin, LOW);
      }
    }

    digitalWrite(csPin, HIGH);
  }

  void flush() {
    for (auto i = 0; i < MX_ROW_SIZE; i++) {  // all data rows
      auto changed = false;                   // set to true if we detected a change
      clearSpiBuffer();

      for (auto dev = 0; dev < devices; dev++) {  // all devices
        if (bitRead(matrix[dev].changed, i)) {
          //  put our device data into the buffer
          spiData[spiOffset(dev, 0)] = MX_DIGIT0 + i;
          spiData[spiOffset(dev, 1)] = matrix[dev].row[i];
          changed = true;
        }
      }
      if (changed) spiSend();
    }

    for (auto dev = 0; dev < devices; dev++)  // reset the device changed flag
      matrix[dev].changed = 0;
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
    this->matrix = new DeviceInfo[this->devices];
    this->spiData = new byte[this->spiDataSize()];
    clear();
    clearSpiBuffer();

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

  void setIntensity(const byte intensity) {
    this->intensity = intensity;
    control(MX_INTENSITY, this->intensity);
  }

  void clear(const byte dev) {
    memset(matrix[dev].row, 0, sizeof(matrix[dev].row));
    matrix[dev].changed = 0xff;
  }

  void clear() {
    for (byte dev = 0; dev < devices; dev++) {
      clear(dev);
    }
  }

  void control(const byte mode, const int val) {
    clearSpiBuffer();
    for (auto dev = 0; dev < devices; dev++)
      controlHardware(dev, mode, val);
    spiSend();
  }

  void update() {
    flush();
  }

  void setRow(const byte dev, const byte row, const byte val) {
    matrix[dev].row[row] = val;
    bitSet(matrix[dev].changed, row);
  }

  void setRow(const byte row, const byte val) {
    for (byte dev = 0; dev < devices; dev++) {
      setRow(dev, row, val);
    }
  }

  void setCol(const byte dev, const byte col, const byte val) {
    for (auto row = 0; row < MX_COL_SIZE; row++) {
      if (val & (1 << row)) {
        bitSet(matrix[dev].row[row], col);
      } else {
        bitClear(matrix[dev].row[row], col);
      }
      bitSet(matrix[dev].changed, row);
    }
  }

  void setCol(const byte col, const byte val) {
    for (byte dev = 0; dev < devices; dev++) {
      setCol(dev, col, val);
    }
  }

  void setPoint(const byte dev, const byte row, const byte col, const bool lit) {
    if (lit) {
      bitSet(matrix[dev].row[row], col);
    } else {
      bitClear(matrix[dev].row[row], col);
    }
    bitSet(matrix[dev].changed, row);
  }

  void setPoint(const byte row, const byte col, const bool lit) {
    for (byte dev = 0; dev < devices; dev++) {
      setPoint(dev, row, col, lit);
    }
  }

  void setPointXY(const byte x, const byte y, const bool lit) {
    byte dev = (devices - 1) - (x >> 3);
    setPoint(dev, 7 - y, 7 - (x & 7), lit);
  }

  void setChar(const unsigned int column, const unsigned int chr) {
    // mx->setChar(column, chr);
  }
};

class MX7219Task : public MX7219, public Task {
 public:
  MX7219Task(const byte pwrPin, const byte dataPin, const byte clkPin, const byte csPin, const byte devices = 1)
      : MX7219(pwrPin, dataPin, clkPin, csPin, devices) {}
};

const byte PROGMEM font[] = {
    5, 62, 81, 73, 69, 62,   // 48 - '0'
    3, 4, 2, 127,            // 49 - '1'
    5, 113, 73, 73, 73, 70,  // 50 - '2'
    5, 65, 73, 73, 73, 54,   // 51 - '3'
    5, 15, 8, 8, 8, 127,     // 52 - '4'
    5, 79, 73, 73, 73, 49,   // 53 - '5'
    5, 62, 73, 73, 73, 48,   // 54 - '6'
    5, 3, 1, 1, 1, 127,      // 55 - '7'
    5, 54, 73, 73, 73, 54,   // 56 - '8'
    5, 6, 73, 73, 73, 62,    // 57 - '9'
    5, 126, 9, 9, 9, 126,    // 65 - 'A'
    5, 127, 73, 73, 73, 54,  // 66 - 'B'
    5, 62, 65, 65, 65, 65,   // 67 - 'C'
    5, 127, 65, 65, 65, 62,  // 68 - 'D'
    5, 127, 73, 73, 73, 65,  // 69 - 'E'
    5, 127, 9, 9, 9, 1,      // 70 - 'F'
    5, 62, 65, 65, 73, 121,  // 71 - 'G'
    5, 127, 8, 8, 8, 127,    // 72 - 'H'
    3, 65, 127, 65,          // 73 - 'I'
    5, 48, 65, 65, 65, 63,   // 74 - 'J'
    5, 127, 8, 20, 34, 65,   // 75 - 'K'
    5, 127, 64, 64, 64, 64,  // 76 - 'L'
    5, 127, 2, 12, 2, 127,   // 77 - 'M'
    5, 127, 4, 8, 16, 127,   // 78 - 'N'
    5, 62, 65, 65, 65, 62,   // 79 - 'O'
    5, 127, 9, 9, 9, 6,      // 80 - 'P'
    5, 62, 65, 65, 97, 126,  // 81 - 'Q'
    5, 127, 9, 25, 41, 70,   // 82 - 'R'
    5, 70, 73, 73, 73, 49,   // 83 - 'S'
    5, 1, 1, 127, 1, 1,      // 84 - 'T'
    5, 63, 64, 64, 64, 63,   // 85 - 'U'
    5, 31, 32, 64, 32, 31,   // 86 - 'V'
    5, 63, 64, 56, 64, 63,   // 87 - 'W'
    5, 99, 20, 8, 20, 99,    // 88 - 'X'
    5, 3, 4, 120, 4, 3,      // 89 - 'Y'
    5, 97, 81, 73, 69, 67,   // 90 - 'Z'
};

#endif