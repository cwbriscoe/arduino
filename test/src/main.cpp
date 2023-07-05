// Copyright 2023 Christopher Briscoe.  All rights reserved.

#include "manager.h"

Manager manager;

void setup() {  // cppcheck-suppress unusedFunction
  SysLib::enableDebuggingSerialPort();
  manager.init();
}

void loop() {  // cppcheck-suppress unusedFunction
  manager.run();
}