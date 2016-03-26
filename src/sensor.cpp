#include <Arduino.h>
#include "sensor.h"

namespace mbot {

const int S1 = A2;
const int S2 = A3;

Sensor::Sensor(int pin) : pin(pin) {
}
void Sensor::setup() {
  pinMode(pin, INPUT);
}
int Sensor::read() {
  return analogRead(pin);
}

}

