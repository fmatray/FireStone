#ifndef TIMER_H
#define TIMER_H
#include <Arduino.h>

#include "sensor.h"

typedef struct {
  uint16_t idle_timeout;
  uint16_t off_timeout;
} timer_settings_t;

class TimerSensor : public Sensor {
 public:
  void begin();
  bool test() { return true; }
  void update(const timer_settings_t _settings) { idle_timeout = _settings.idle_timeout; };
  void run();
  bool read();
  status_e check();
  void clear() { last_update = millis(); };

 private:
  unsigned long last_update = 0;
  uint16_t idle_timeout     = 5;
  uint16_t off_timeout      = 5;
};

#endif
