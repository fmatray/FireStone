#include "controler/controler.h"

#include "common/helpers.h"
#include "controler/ambiant.h"
#include "controler/aq.h"
#include "controler/emergency.h"
#include "controler/fire.h"
#include "controler/octoprint.h"
#include "controler/timer.h"


void Controler::begin() {
  title("Controler Setup");
#ifndef EMERGENCY_INVERTED
  pinMode(EMERGENCY_RESET_PIN, INPUT_PULLDOWN);
#else
  pinMode(EMERGENCY_RESET_PIN, INPUT_PULLUP);
#endif
  for (uint8_t i = 0; i < RULES_SIZE; i++)
    rules[i] = NULL;
  rules[0] = new FireRule();
  rules[1] = new EmergencyRule();
  rules[2] = new AirQualityRule();
  rules[3] = new OctoprintRule();
  rules[4] = new AmbiantRule();
  rules[5] = new TimerRule();

  for (uint8_t i = 0; i < RULES_SIZE && rules[i]; i++)
    rules[i]->begin();
  reset();
}

void Controler::reset() {
  dispatcher.reset_or_wakeup();
  display.reset();
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
                 digitalRead(EMERGENCY_RESET_PIN), RESET_INTERVAL) &&
#ifdef EMERGENCY_INVERTED
        reset_button_state == false)
#else
        reset_button_state == true)
#endif
    {
      reset();
      return;
    }
  }
  for (unsigned int i = 0; i < RULES_SIZE && rules[i]; i++)
    if (rules[i]->is_active() && rules[i]->proceed(dispatcher))
      break;

  dispatcher.dispatch();
}
