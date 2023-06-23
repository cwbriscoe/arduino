#ifndef LED_H
#define LED_H

void setupLED(const byte pin) {
  pinMode(pin, OUTPUT);
}

void turnOnLED(const byte pin) {
  digitalWrite(pin, HIGH);
}

void turnOffLED(const byte pin) {
  digitalWrite(pin, LOW);
}

#endif