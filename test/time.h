#ifndef TIME_H
#define TIME_H

#include "debug.h"

class Time {
private:
  unsigned long time = 0;
  unsigned long generation = 0;

public:
  Time() {
    this->update();
  }

  unsigned long micros() const {
    return time;
  }
  unsigned long gen() const {
    return generation;
  }

  void add(const unsigned long duration) {
    assert(duration < MAX_TIME);
    this->update();
    if (MAX_TIME - this->time < duration) {
      this->time = duration - (MAX_TIME - this->time);
      this->generation++;
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
      generation++;
    }
    time = currTime;
  }

  friend bool operator<(const Time& lhs, const Time& rhs) {
    if (lhs.generation < rhs.generation) { return true; }
    if (lhs.generation > rhs.generation) { return false; }
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
    this->generation = rhs.generation;
    return *this;
  }
};

class Trigger : public Time {
  private:
    unsigned long interval;

  public:
    Trigger(const unsigned long interval, const bool immediate = false ) : Time() {
      assert(interval < MAX_TIME);
      this->interval = interval;
      if (!immediate) { this->add(this->interval); }
    }

    bool triggered(const Time& time) {
      auto ret = (*this <= time);
      if (ret) { this->add(this->interval); }
      return ret;
    }
};

// now() updates our global time object (to keep track of generation), updates
// to the current time and returns a copy
Time _time;
const Time& now() {
  _time.update();
  return _time;
}

unsigned long getTime() {
#ifdef DEBUG
  return miniMicros();
#else
  return micros();
#endif
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