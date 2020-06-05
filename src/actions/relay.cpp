#include "IO/relay.h"

#include <Arduino.h>

#include "common/helpers.h"

RelayAction::RelayAction(uint8_t _pin) {
  pin = _pin;
  pinMode(pin, OUTPUT);
}

void RelayAction::update(const relay_settings_t _settings) {
  inverted      = _settings.inverted;
  switch_status = _settings.switch_status;
}

void RelayAction::run() {
  if (action >= switch_status) {
    if (inverted)
      turn_on();
    else
      turn_off();
    return;
  }
  if (inverted)
    turn_off();
  else
    turn_on();
}

bool RelayAction::get_state() { return state; }
String RelayAction::get_state_as_str() { return state ? "on " : "off"; }

void RelayAction::turn_on() {
  digitalWrite(pin, HIGH);
  state = true;
}

void RelayAction::turn_off() {
  digitalWrite(pin, LOW);
  state = false;
}