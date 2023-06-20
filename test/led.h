#ifndef LED_H
#define LED_H

void setupLED(byte pin) {
  pinMode(pin, OUTPUT);
}

void powerOnLED(byte pin) {
  digitalWrite(pin, HIGH);
}

void powerOffLED(byte pin) {
  digitalWrite(pin, LOW);
}

#endif