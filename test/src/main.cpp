// Copyright 2023 Christopher Briscoe.  All rights reserved.

#include "init.h"

void setup() {
  initialize();
}

void loop() {
  threadManager.run();
}