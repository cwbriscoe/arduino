// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef TIME_H
#define TIME_H

#include "debug.h"

class Time {
 private:
  unsigned long time = 0;  // time in microseconds
  unsigned int gens = 0;   // how many times time has overflowed

 public:
  Time() {
    this->update();
  }

  unsigned long microseconds() const {
    return time;
  }
  unsigned long generations() const {
    return gens;
  }

  void add(const unsigned long duration) {
    assert(duration < MAX_TIME);
    this->update();
    if (MAX_TIME - this->time < duration) {
      this->time = duration - (MAX_TIME - this->time);
      this->gens++;
      return;
    }
    this->time += duration;
  }

  void update() {
#ifdef DEBUG
    auto currTime = miniMicros();
#else
    auto currTime = micros();
#endif
    if (currTime < time) {
      gens++;
    }
    time = currTime;
  }

  friend bool operator<(const Time& lhs, const Time& rhs) {
    if (lhs.gens < rhs.gens) { return true; }
    if (lhs.gens > rhs.gens) { return false; }
    return (lhs.time < rhs.time);
  }
  friend bool operator>(const Time& lhs, const Time& rhs) {
    return rhs < lhs;
  }
  friend bool operator<=(const Time& lhs, const Time& rhs) {
    return !(lhs > rhs);
  }
  friend bool operator>=(const Time& lhs, const Time& rhs) {
    return !(lhs < rhs);
  }

  Time& operator=(const Time& rhs) {
    this->time = rhs.time;
    this->gens = rhs.gens;
    return *this;
  }
};

class Trigger : public Time {
 private:
  unsigned long interval;  // interval between trigger events
  bool automatic;          // whether interval is re-added after a trigger event or manually reset()
  bool executed;           // keeps track of the triggered/reset cycle when automatic is false

 public:
  explicit Trigger(const unsigned long interval, const bool immediate = false, const bool automatic = true)
      : Time() {
    assert(interval < MAX_TIME);
    this->interval = interval;
    this->automatic = automatic;
    this->executed = false;
    if (!immediate) {
      this->add(this->interval);
    }
  }

  bool triggered(const Time& time) {
    assert(this->automatic || !this->executed);
    auto ret = (*this <= time);
    if (ret) {
      if (this->automatic) { this->add(this->interval); }
      this->executed = true;
    }
    return ret;
  }

  void reset(const unsigned long interval) {
    assert(this->automatic == false);
    assert(this->executed == true);
    this->interval = interval;
    this->add(this->interval);
    this->executed = false;
  }

  unsigned long next(const Time& time) {
    assert(this->generations() >= time.generations());
    assert(*this >= time);
    if (this->generations() > time.generations()) {
      return (MAX_TIME - time.microseconds()) + this->microseconds();
    }
    return this->microseconds() - time.microseconds();
  }
};

// now() updates our global time object (to keep track of generation), updates
// to the current time and returns a copy
Time _time;
const Time& now() {
  _time.update();
  return _time;
}

// according to the official docs at https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/
// the minimum accurate delay for delayMicroseconds() is 16383.  This function will delay anything over 1 millisecond
// using delay() and use delayMicroseconds for the remaining time
void wait(const unsigned long microseconds) {
  auto milliseconds = microseconds / 1000;
  delay(milliseconds);
  delayMicroseconds(microseconds - (milliseconds * 1000));
}

#endif