#include "env.h"
#include "init.h"

void setup() {
  //init our code for first time use
  initialize();

  // setup pin configs
  setupLED(LED_BUILTIN);

  // if debuggin open serial communication over USB
#ifdef DEBUG
  Serial.begin(9600);
#endif
}

void loop() {
  threadManager.run();
}