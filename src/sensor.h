#ifndef SENSOR_H
#define SENSOR_H

namespace mbot {

extern const int S1;
extern const int S2;

class Sensor {
public:
  Sensor(int pin);

  void setup();
  int read();
private:
  int pin;
};

}

#endif /* end of include guard: SENSOR_H */
