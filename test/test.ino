#include "init.h"

void setup() {
  //init our code for first time use
  initialize();
}

void loop() {
  threadManager.run();
}