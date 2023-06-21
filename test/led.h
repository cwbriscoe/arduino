#ifndef LED_H
#define LED_H

void setupLED(const byte pin) {
  pinMode(pin, OUTPUT);
}

void powerOnLED(const byte pin) {
  digitalWrite(pin, HIGH);
}

void powerOffLED(const byte pin) {
  digitalWrite(pin, LOW);
}

#endif