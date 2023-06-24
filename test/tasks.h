// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef TASKS_H
#define TASKS_H

#include "debug.h"
#include "led.h"
#include "mx7219.h"

/*****************************************************************************/
/* Blink Task                                                                */
/*****************************************************************************/
auto blinkStatus = false;
void taskBlink(const Time&) {
  if (blinkStatus) {
    turnOnLED(LED_BUILTIN);
  } else {
    turnOffLED(LED_BUILTIN);
  }

  blinkStatus = !blinkStatus;
}

/*****************************************************************************/
/* Count Task                                                                */
/*****************************************************************************/
Trigger countTrigger((unsigned long)1000 * 1000, true);
void taskCount(const Time& time) {
  if (countTrigger.triggered(time)) {
    Serial.print("taskCount time: ");
    Serial.println(time.microseconds());
  }
}

/*****************************************************************************/
/* Max7219 Task                                                              */
/*****************************************************************************/
#define MAX_DEVICES 4
#define CS_PIN 3
#define DATA_PIN 2
#define CLK_PIN 4

mx7219 mx(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

void taskMax7219Setup() {
  mx.begin();
  // Set the intensity (brightness) of the display (0-15):
  mx.setIntensity(0);
  // Clear the display:
  mx.clear();
}

byte mxi = 0;
Trigger max7219Trigger(0, true, false);
void taskMax7219(const Time& time) {
  if (!max7219Trigger.triggered(time)) { return; }

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
  max7219Trigger.reset((unsigned long)250 * 1000);
}

/*
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN 3
#define DATA_PIN 2
#define CLK_PIN 4
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);  //SPI setup
//MD_MAX72XX(moduleType_t mod, uint8_t dataPin, uint8_t clkPin, uint8_t csPin, uint8_t numDevices=1);

void taskMax7219Setup() {
  myDisplay.begin();
  // Set the intensity (brightness) of the display (0-15):
  myDisplay.setIntensity(0);
  // Clear the display:
  myDisplay.displayClear();
}

byte stage = 0;
Trigger max7219Trigger(0, true, false);
void taskMax7219(const Time& time) {
  if (!max7219Trigger.triggered(time)) { return; }

  switch (stage) {
    case 0:
      myDisplay.setTextAlignment(PA_CENTER);
      myDisplay.print("Center");
      max7219Trigger.reset((unsigned long)2000 * 1000);
      break;
    case 1:
      myDisplay.setTextAlignment(PA_LEFT);
      myDisplay.print("Left");
      max7219Trigger.reset((unsigned long)2000 * 1000);
      break;
    case 2:
      myDisplay.setTextAlignment(PA_RIGHT);
      myDisplay.print("Right");
      max7219Trigger.reset((unsigned long)2000 * 1000);
      break;
    case 3:
      myDisplay.print(1234);
      max7219Trigger.reset((unsigned long)2000 * 1000);
      break;
  }

  stage++;
  if (stage > 3) { stage = 0;}
}
*/

#endif