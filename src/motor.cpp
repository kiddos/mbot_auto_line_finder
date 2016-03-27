#include <Arduino.h>
#include "motor.h"

namespace mbot {

const MotorPin M1 = {7, 6};
const MotorPin M2 = {4, 5};

Motor::Motor(MotorPin pin) : pin(pin) {
}
void Motor::setup() {
  pinMode(pin.dirpin, OUTPUT);
  pinMode(pin.speedpin, OUTPUT);
}
void Motor::run(int speed) {
  int dir = HIGH;
  if (speed < 0) {
    dir = LOW;
    speed = -speed;
  }
  digitalWrite(pin.dirpin, dir);
  analogWrite(pin.speedpin, speed);
}
void Motor::stop() {
  analogWrite(pin.speedpin, 0);
}

}

