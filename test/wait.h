#ifndef WAIT_H
#define WAIT_H

// according to the official docs at https://www.arduino.cc/reference/en/language/functions/time/delaymicroseconds/
// the minimum accurate delay for delayMicroseconds() is 16383.  This function will delay anything over 1 millisecond
// using delay() and use delayMicroseconds for the remaining time
void wait(const unsigned long microseconds) {
  auto milliseconds = microseconds / 1000;
  delay(milliseconds);
  delayMicroseconds(microseconds - (milliseconds * 1000));
}

#endif