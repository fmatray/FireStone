#include "IO/emergency.h"

#include "common/helpers.h"

/*
   SETUP
*/

EmergencySensor::EmergencySensor() {
  emergency_state = false;
}

void EmergencySensor::begin() {
  title("Emergency Sensor Setup");
#ifndef EMERGENCY_INVERTED
  pinMode(EMERGENCY_PIN, INPUT_PULLDOWN);
#else
  pinMode(EMERGENCY_PIN, INPUT_PULLUP);
#endif
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
               digitalRead(EMERGENCY_PIN), EMERGENCY_INTERVAL)) {
#ifndef EMERGENCY_INVERTED
    emergency_state = button_state;
#else
    emergency_state = !button_state;
#endif
    return true;
  }
  return false;
}

status_e EmergencySensor::check() {
  if (emergency_state == true)
    return ALERT;
  return OK;
}
