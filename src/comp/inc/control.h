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

  virtual void update() {}

  inline void setEnabled() { enabled = true; }
  inline void setDisabled() { enabled = false; }

  inline bool toggleEnabled() {
    enabled = !enabled;
    return enabled;
  }

  inline bool isEnabled() const { return enabled; }
  inline bool isDisabled() const { return !enabled; }
};
}  // namespace SysLib

#endif