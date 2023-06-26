// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef CONTROL_H
#define CONTROL_H

#include "env.h"

class Control {
 private:
  bool enabled = true;

 public:
  Control() {}

  void setEnabled(const bool enabled) {
    if (this->enabled != enabled) {
      this->enabled = enabled;
      this->onEnabledChanged();
    }
  }

  inline bool isEnabled() { return enabled; }

  virtual void onEnabledChanged() {}
};

#endif