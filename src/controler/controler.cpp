#include "controler/controler.h"

#include "controler/ambiant.h"
#include "controler/emergency.h"
#include "controler/fire.h"
#include "controler/octoprint.h"
#include "helpers.h"

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

  for (unsigned int i = 0; i < RULES_SIZE && rules[i]; i++)
    rules[i]->begin();
  reset();
}

void Controler::reset() {
  actions = {OK,      // Global
             OK, OK,  // Buzzer, Octoprint
             OK, OK,  // Relay 1, 2,
             OK, OK,  // Relay 3,4
             ""};     // Message
  ambiant_sensor.reset();
  fire_sensor.reset();
  emergency_sensor.reset();
  octoprint_sensor.reset();
}

void Controler::run() {
  static unsigned long reset_lasttime = 0;
  static bool reset_last_state        = false;
  static bool reset_button_state      = false;

  if (actions.global > OK) {
    if (debounce(&reset_lasttime, &reset_last_state, &reset_button_state,
                 digitalRead(reset_pin), RESET_INTERVAL) &&
        reset_button_state == true)
      reset();
  } else
    for (unsigned int i = 0; i < RULES_SIZE && rules[i]; i++)
      if (rules[i]->is_active() && rules[i]->proceed(actions))
        break;

  dispatch(actions);
}

void Controler::dispatch(const actions_t actions) {
  display.set_status(actions.global, actions.message);
  buzzer.set_status(actions.buzzer);
  //octoprint_action.set(actions.global, actions.octoprint);
  relay1.set_status(actions.relay1);
  relay2.set_status(actions.relay1);
  relay3.set_status(actions.relay1);
  relay4.set_status(actions.relay1);
}

void Controler::print_actions(const actions_t actions) {
  title("Actions");
  Serial.print("Global:");
  Serial.println(status_str(actions.global));
  Serial.print("Message:");
  Serial.println(actions.message);
}