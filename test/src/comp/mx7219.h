// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef mx7219_H
#define mx7219_H

#include <MD_MAX72xx.h>

#include "env.h"

// control registers (1-8 = digits 0-7)
#define MX_NOOP 0
#define MX_DECODEMODE 9
#define MX_INTENSITY 10
#define MX_SCANLIMIT 11
#define MX_SHUTDOWN 12
#define MX_DISPLAYTEST 15

// other constants
#define MX_ROW_SIZE 8
#define MX_COL_SIZE 8

class MX7219 {
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
  MD_MAX72XX* mx;      // wrapping MD_MAX72XX library for now

  inline byte spiDataSize() const {
    return sizeof(byte) * this->devices * 2;
  }

 public:
  MX7219(const byte pwrPin, const byte dataPin, const byte clkPin, const byte csPin, const byte devices = 1) {
    this->pwrPin = pwrPin;
    this->dataPin = dataPin;
    this->clkPin = clkPin;
    this->csPin = csPin;
    this->devices = devices;
    this->intensity = 0;

    // allocate memory needed for buffers
    this->matrix = new DeviceInfo[this->devices];
    this->spiData = new byte[this->spiDataSize()];

    // set all pins to output
    pinMode(this->pwrPin, OUTPUT);
    pinMode(this->dataPin, OUTPUT);
    pinMode(this->clkPin, OUTPUT);
    pinMode(this->csPin, OUTPUT);
    digitalWrite(this->csPin, HIGH);
    digitalWrite(this->pwrPin, HIGH);

    // initialize driver and turn auto updates off
    mx = new MD_MAX72XX(MD_MAX72XX::FC16_HW, this->dataPin, this->clkPin, this->csPin, this->devices);
    mx->control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  }

  void setIntensity(const byte intensity) {
    this->intensity = intensity;
    mx->control(MD_MAX72XX::INTENSITY, this->intensity);
  }

  bool begin() {
    return mx->begin();
  }

  void clear() {
    mx->clear();
  }

  void clear(const byte device) {
    mx->clear(device);
  }

  void update() {
    mx->update();
  }

  void setChar(const unsigned int column, const unsigned int chr) {
    mx->setChar(column, chr);
  }
};

#endif