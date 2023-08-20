// Copyright 2023 Christopher Briscoe.  All rights reserved.

#include "proj/test/manager.h"

App::Manager manager;

void setup() {  // cppcheck-suppress unusedFunction
  SysLib::enableDebuggingSerialPort();
  manager.init();

  // init random seed
  auto seed = SysLib::initRandomSeed();
  print(F("setup complete : seed = "));
  println(seed);
  (void)seed;  // prevent compiler warning
}

void loop() {  // cppcheck-suppress unusedFunction
  manager.run();
}