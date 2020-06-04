#include "sensors/emergency.h"

#include "common/helpers.h"

/*
   SETUP
*/

EmergencySensor::EmergencySensor(const uint8_t _emergency_pin) {
  emergency_pin   = _emergency_pin;
  emergency_state = false;
}

void EmergencySensor::begin() {
  title("Emergency Sensor Setup");
  pinMode(emergency_pin, INPUT_PULLDOWN);
}

/*
   LOOP
*/
void EmergencySensor::run() {
  if (read())
    status = check();
}

bool EmergencySensor::read() {
  static unsigned long emergency_lasttime = 0;
  static bool last_state                  = false;
  static bool button_state                = false;

  if (debounce(&emergency_lasttime, &last_state, &button_state,
               digitalRead(emergency_pin), EMERGENCY_INTERVAL)) {
    emergency_state = button_state;
    return true;
  }
  return false;
}

status_e EmergencySensor::check() {
  if (emergency_state == true)
    return ALERT;
  return OK;
}
