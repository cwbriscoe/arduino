// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef CONTROL_H
#define CONTROL_H

#include "env.h"

class Control {
 private:
  bool enabled = true;
  Control* listener = nullptr;

 public:
  Control() {}

  inline void setEnabled(const bool enabled) { this->enabled = enabled; }

  inline void toggleEnabled() { enabled = !enabled; }

  inline bool isEnabled() { return enabled; }

  inline void addListener(Control* listener) { this->listener = listener; }

  void notifyListeners(unsigned long value) {
    if (!listener) return;
    listener->onControllerChanged(value);
  }

  virtual void onControllerChanged(const unsigned long value) {}
};

#endif