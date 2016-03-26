#ifndef MOTOR_H
#define MOTOR_H

namespace mbot {

struct MotorPin {
  int dirpin, speedpin;
};

extern const MotorPin M1;
extern const MotorPin M2;

class Motor {
 public:
  Motor(MotorPin pin);

  void setup();
  void run(int speed);
  void stop();
 private:
  MotorPin pin;
};

}


#endif /* end of include guard: MOTOR_H */

