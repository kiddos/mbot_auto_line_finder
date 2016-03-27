#include "ultrasonic.h"

namespace mbot {

const int U1 = 10;

Ultrasonic::Ultrasonic(int pin) : pin(pin) {
}
double Ultrasonic::distanceCM(unsigned int maxdis) {
  long dist = measure(maxdis * 55 + 200);
  return (double) dist / 58.0;
}
long Ultrasonic::measure(unsigned long timeout) {
  long dur = 0;
  pinMode(pin, OUTPUT);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin, LOW);

  pinMode(pin, INPUT);
  dur = pulseIn(pin, HIGH, timeout);
  return dur;
}

}
