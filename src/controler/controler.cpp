#include "controler/controler.h"

#include "common/helpers.h"
#include "controler/ambiant.h"
#include "controler/emergency.h"
#include "controler/fire.h"
#include "controler/octoprint.h"

Actions action_reset = {off,       // display
                        off, off,  // Buzzer, Octoprint
                        off, off,  // Relay 1, 2,
                        off, off,  // Relay 3,4
                        ""};

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
  actions = action_reset;
  ambiant_sensor.reset();
  fire_sensor.reset();
  emergency_sensor.reset();
  octoprint.reset();
}

void Controler::run() {
  static unsigned long reset_lasttime = 0;
  static bool reset_last_state        = false;
  static bool reset_button_state      = false;

  if (actions != action_reset) {
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

void Controler::dispatch(const Actions actions) {
  display.set(actions.display, actions.message);
  buzzer.set(actions.buzzer);
  octoprint.set(actions.octoprint);
  relay1.set(actions.relay1);
  relay2.set(actions.relay1);
  relay3.set(actions.relay1);
  relay4.set(actions.relay1);
}

void Controler::print_actions(const Actions actions) {
  title("Actions");
  Serial.print("Display:");
  Serial.println(action_str(actions.display));
  Serial.print("Buzzer:");
  Serial.println(action_str(actions.buzzer));
  Serial.print("Relay1:");
  Serial.println(action_str(actions.relay1));
  Serial.print("Relay2:");
  Serial.println(action_str(actions.relay2));
  Serial.print("Relay3:");
  Serial.println(action_str(actions.relay3));
  Serial.print("Relay4:");
  Serial.println(action_str(actions.relay4));
  Serial.print("Message:");
  Serial.println(actions.message);
}