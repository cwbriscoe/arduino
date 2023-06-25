// Copyright 2023 Christopher Briscoe.  All rights reserved.

#include "manager.h"

Manager manager;

void setup() {
  manager.init();
}

void loop() {
  manager.run();
}