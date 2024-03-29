#include "controler/dispatcher.h"

#include <Arduino.h>

#include "common/helpers.h"

void Dispatcher::reset() {
  general = nothing;
  buzz    = nothing;
  message = "";
}
bool Dispatcher::need_reset() {
  return general == off || general == sleep || general == warn || general == alert;
}

void Dispatcher::set(const action_e _general, const action_e _buzz, const String _msg) {
  if (general >= _general && buzz >= _buzz)
    return;
  general = _general;
  buzz    = _buzz;
  message = _msg;
}

void Dispatcher::dispatch() {
  if (last_general == general && last_buzz == buzz)
    return;
  last_general = general;
  last_buzz    = buzz;

  display.set(general, message);
  buzzer.set(buzz);
  octoprint.set(general);
  /* !!!!!! DISABLED FOR NOW
  relay1.set(general);
  relay2.set(general);
  relay3.set(general);
  relay4.set(general);
  */
}

void Dispatcher::print() {
  title("Dispatcher");
  Serial.print("Global:");
  Serial.println(action_str(general));
  Serial.print("Buzzer:");
  Serial.print("Message:");
  Serial.println(message);
}

void Dispatcher::reset_or_wakeup() {
  if (this->is_off_or_sleeping()) {
    general = wakeup;
    dispatch();
  }
  reset();
}