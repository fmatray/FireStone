#include "IO/fire.h"

#include "common/helpers.h"

/*
   SETUP
*/

FireSensor::FireSensor(const uint8_t _pin) {
  pin = _pin;
}

void FireSensor::begin() {
  title("Fire Sensor Setup");
  pinMode(pin, INPUT_PULLDOWN);
}
/*
   LOOP
*/
void FireSensor::run() {
  if (read())
    status = check();
}

bool FireSensor::read() {
  static unsigned long fire_lasttime = 0;
  static bool last_state             = false;
  static bool button_state           = false;

  if (debounce(&fire_lasttime, &last_state, &button_state,
               digitalRead(pin), FIRE_INTERVAL)) {
    fire_state = button_state;
    return true;
  }
  return false;
}

status_e FireSensor::check() {
  if (fire_state == true)
    return ALERT;
  return OK;
}
