#include "IO/timer.h"

#include <Arduino.h>

#include "common/helpers.h"
void TimerSensor::begin() { last_update = millis(); }

void TimerSensor::run() { status = check(); };

bool TimerSensor::read() { return true; };

status_e TimerSensor::check() { return check_time(last_update, timeout * 60) ? IDLE : OK; }