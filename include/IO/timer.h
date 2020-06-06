#ifndef TIMER_H
#define TIMER_H
#include <Arduino.h>

#include "sensor.h"

typedef struct {
  uint16_t timeout;
} timer_settings_t;

class TimerSensor : public Sensor {
 public:
  void begin();
  bool test() { return true; }
  void update(const timer_settings_t _settings) { timeout = _settings.timeout; };
  void run();
  bool read();
  status_e check();
  void clear() { last_update = millis(); };

 private:
  unsigned long last_update = 0;
  uint16_t timeout          = 5;
};

#endif
