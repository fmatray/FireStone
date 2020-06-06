#include "controler/controler.h"

#include "common/helpers.h"
#include "controler/ambiant.h"
#include "controler/emergency.h"
#include "controler/fire.h"
#include "controler/octoprint.h"
#include "controler/time.h"

Controler::Controler(const uint8_t _reset_pin) { reset_pin = _reset_pin; }

void Controler::begin() {
  title("Controler Setup");
  pinMode(reset_pin, INPUT_PULLDOWN);
  for (unsigned int i = 0; i < RULES_SIZE; i++)
    rules[i] = NULL;
  rules[0] = new FireRule();
  rules[1] = new EmergencyRule();
  rules[2] = new OctoprintRule();
  rules[3] = new AmbiantRule();
  rules[4] = new TimeRule();

  for (unsigned int i = 0; i < RULES_SIZE && rules[i]; i++)
    rules[i]->begin();
  reset();
}

void Controler::reset() {
  dispatcher.reset_or_wakeup();
  ambiant_sensor.reset();
  fire_sensor.reset();
  emergency_sensor.reset();
  octoprint.reset();
  timer.clear();
}

void Controler::run() {
  static unsigned long reset_lasttime = 0;
  static bool reset_last_state        = false;
  static bool reset_button_state      = false;

  if (dispatcher.need_reset()) {
    if (debounce(&reset_lasttime, &reset_last_state, &reset_button_state,
                 digitalRead(reset_pin), RESET_INTERVAL) &&
        reset_button_state == true)
      reset();
  } else
    for (unsigned int i = 0; i < RULES_SIZE && rules[i]; i++)
      if (rules[i]->is_active() && rules[i]->proceed(dispatcher))
        break;

  dispatcher.dispatch();
}
