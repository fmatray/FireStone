#include "IO/timer.h"

#include <Arduino.h>

#include "common/helpers.h"
void TimerSensor::begin() { last_update = millis(); }

void TimerSensor::run() { status = check(); };

bool TimerSensor::read() { return true; };

status_e TimerSensor::check() {
  if (idle_timeout == 0 && off_timeout == 0)
    return OK;

  if (check_time(last_update, idle_timeout * 60)) {
    if (off_timeout && check_time(last_update, (idle_timeout + off_timeout) * 60))
      return OFF;
    else
      return IDLE;
  }
  return OK;
}