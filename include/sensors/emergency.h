#ifndef EMERGENCY_H
#define EMERGENCY_H
#include <Arduino.h>

#include "sensor.h"
#define EMERGENCY_INTERVAL 100   // 100ms debounce

class EmergencySensor : public Sensor {
 public:
  EmergencySensor(const uint8_t _emergency_pin);
  void begin();
  bool test() { return true; }
  void run();
  bool read();
  status_e check();

 private:
  bool emergency_state;
  uint8_t emergency_pin;
};

#endif
