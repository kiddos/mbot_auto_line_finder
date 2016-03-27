#include <Arduino.h>

#include "motor.h"
#include "sensor.h"
#include "ultrasonic.h"
#include "blackwhitedetector.h"

using mbot::Motor;
using mbot::Sensor;
using mbot::Ultrasonic;

Motor motor1(mbot::M1);
Motor motor2(mbot::M2);
Sensor sensor1(mbot::S1);
Sensor sensor2(mbot::S2);
Ultrasonic ultra(mbot::U1);
BlackWhiteDetector bwd;

const int trainingstage = 200;
const int setupiter = 600;
int maxiter = 16;
long iter = 0;
long spintime = 100;
int spinspeed = 60;

void turn() {
  int dur = random(1000);

  motor1.run(70);
  motor2.run(70);

  delay(1000 + dur);

  motor1.run(0);
  motor2.run(0);
}

void move() {
  motor1.run(70);
  motor2.run(-70);
}

void setup() {
  motor1.setup();
  motor2.setup();
  sensor1.setup();
  sensor2.setup();

  Serial.begin(9600);
  randomSeed(analogRead(0));

  for (int i = 0 ; i < setupiter ; ++i) {
    const float x = random(10000) / 50000.0f;
    bwd.feeddata(x, i % (setupiter / 10) == 0);
    bwd.feeddata(1-x, false);
  }
  Serial.println("ready to start!!");
}

void loop() {
  float value1 = sensor1.read() / 1024.0f;
  float value2 = sensor2.read() / 1024.0f;
  float a1 = 0, a2 = 0;

  for (int i = 0 ; i < maxiter ; ++i) {
    bwd.feeddata(value1, iter % 10 == 0 && i == 0);
    bwd.feeddata(1-value1, iter % 10 == 0 && i == 0);
  }
  for (int i = 0 ; i < maxiter ; ++i) {
    bwd.feeddata(value2, false);
    bwd.feeddata(1-value2, false);
  }

  double dis = ultra.distanceCM(500);

  /*Serial.print("distance: ");*/
  /*Serial.println(dis);*/

  if (iter < trainingstage) {
    if (dis > 0 && dis <= 10.0) {
      turn();
    } else {
      move();
    }
  } else {
    label label1 = bwd.predict(value1, a1, a2);
    label label2 = bwd.predict(value2, a1, a2);
    if (label1 == BLACK && label2 == BLACK) {
      spintime = 100;
      motor1.run(60);
      motor2.run(-60);
    } else if (label1 == BLACK && label2 == WHITE) {
      spintime = 100;
      motor1.run(80);
      motor2.run(60);
    } else if (label1 == WHITE && label2 == BLACK) {
      spintime = 100;
      motor1.run(-60);
      motor2.run(-80);
    } else {
      motor1.run(spinspeed);
      motor2.run(spinspeed);
      delay(spintime);

      motor1.run(0);
      motor2.run(0);

      spinspeed = -spinspeed;
      spintime += 20;
    }
  }

  // evaluate
  if (iter % 100 == 0) {
    label label1 = bwd.predict(0.04, a1, a2);
    label label2 = bwd.predict(0.96, a1, a2);
    Serial.print("\r\nevaluate 0.04 as: ");
    Serial.println((label1 == BLACK ? "BLACK" : "WHITE"));
    Serial.print("evaluate 0.96 as: ");
    Serial.println((label2 == BLACK ? "BLACK" : "WHITE"));
  }

  ++ iter;
  Serial.print("\r\niter: ");
  Serial.print(iter);

  if (iter > 200)
    maxiter = 2;

  /*delay(400);*/
}
