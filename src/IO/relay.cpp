#include "IO/relay.h"

#include <Arduino.h>

#include "common/helpers.h"

#define TURN_OFF  \
  {               \
    if (inverted) \
      turn_on();  \
    else          \
      turn_off(); \
    return;       \
  }

#define TURN_ON   \
  {               \
    if (inverted) \
      turn_off(); \
    else          \
      turn_on();  \
    return;       \
  }

RelayAction::RelayAction(uint8_t _pin) {
  pin = _pin;
  pinMode(pin, OUTPUT);
}

void RelayAction::update(const relay_settings_t _settings) {
  inverted      = _settings.inverted;
  switch_action = _settings.switch_action;
}

void RelayAction::run() {
  if (action == sleep)
    TURN_OFF;
  if (action == wakeup)
    TURN_ON;
  if (action >= switch_action)
    TURN_OFF;
  TURN_ON;
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