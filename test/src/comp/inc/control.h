// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef CONTROL_H
#define CONTROL_H

#include "env.h"

class Control {
 private:
  bool enabled = true;

 public:
  Control() {}

  inline void setEnabled(const bool enabled) { this->enabled = enabled; }

  inline void toggleEnabled() { enabled = !enabled; }

  inline bool isEnabled() const { return enabled; }
};

#endif