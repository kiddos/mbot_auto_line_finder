#include <Arduino.h>

namespace mbot {

extern const int U1;

class Ultrasonic {
public:
  Ultrasonic(int pin);
  double distanceCM(unsigned int maxdis);
private:
  long measure(unsigned long timeout);
  int pin;
};

}
