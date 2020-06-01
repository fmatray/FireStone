#ifndef FIRE_SENSOR_H
#define FIRE_SENSOR_H
#include <Arduino.h>

#include "sensor.h"

#define FIRE_INTERVAL 2000  // 2s debounce to avoid false positive

class FireSensor : public Sensor {
 public:
  FireSensor(const uint8_t pin);
  void begin();
  bool test() { return true; }
  void run();
  bool read();
  status_e check();

 private:
  bool fire_state;
  uint8_t pin;
};

#endif
