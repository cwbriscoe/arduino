#include "init.h"

void setup() {
  //init our code for first time use
  initialize();

  // setup pin configs
  setupLED(LED_BUILTIN);
}

void loop() {
  //threadManager.run();
  task3(time());
  wait(500000);
}