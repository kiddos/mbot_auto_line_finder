#include <Arduino.h>

#include "motor.h"
#include "sensor.h"
#include "blackwhitedetector.h"

using mbot::Motor;
using mbot::Sensor;

Motor motor1(mbot::M1);
Motor motor2(mbot::M2);
Sensor sensor1(mbot::S1);
Sensor sensor2(mbot::S2);
BlackWhiteDetector bwd;

void setup() {
  motor1.setup();
  motor2.setup();
  sensor1.setup();
  sensor2.setup();

  Serial.begin(9600);
}

void loop() {
  Serial.print("Sensor1: ");
  Serial.print(sensor1.read());
  Serial.print(" ");
  Serial.print("Sensor2: ");
  Serial.println(sensor2.read());

  delay(400);
}
