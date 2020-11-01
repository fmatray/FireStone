#ifndef AQ_H
#define AQ_H

#include <Arduino.h>

#include "sensor.h"

class AirQuality : public Sensor {
 public:
  void begin();
  bool test();
  void run();
  bool read();
  status_e check();

 private:

};

#endif