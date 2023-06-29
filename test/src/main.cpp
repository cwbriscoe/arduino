// Copyright 2023 Christopher Briscoe.  All rights reserved.

#include "manager.h"

Manager* manager;

void setup() {
  enableDebuggingSerialPort();
  manager = new Manager();
  manager->init();
}

void loop() {
  manager->run();
}