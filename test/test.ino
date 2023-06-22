#include "init.h"

void setup() {
  //init our code for first time use
  initialize();

  // setup pin configs
  setupLED(LED_BUILTIN);
}

void loop() {
  //threadManager.run();
  task3(getTime());
  //wait(500000);
}