// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef CONTROL_H
#define CONTROL_H

#include "lib/debug.h"

namespace SysLib {
class Control {
 private:
  bool enabled = true;

 public:
  Control() {}

  inline void setEnabled(const bool enabled) { this->enabled = enabled; }

  inline void toggleEnabled() { enabled = !enabled; }

  inline bool isEnabled() const { return enabled; }
};
}  // namespace SysLib

#endif