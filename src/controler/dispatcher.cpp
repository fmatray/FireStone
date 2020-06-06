#include "controler/dispatcher.h"

#include <Arduino.h>

#include "common/helpers.h"

void Dispatcher::reset() {
  general = nothing;
  buzz    = nothing;
  message = "";
}
bool Dispatcher::need_reset() {
  return general == sleep || general == warn || general == alert;
}

void Dispatcher::dispatch() {
  display.set(general, message);
  buzzer.set(buzz);
  octoprint.set(general);
  relay1.set(general);
  relay2.set(general);
  relay3.set(general);
  relay4.set(general);
}

void Dispatcher::print() {
  title("Dispatcher");
  Serial.print("Global:");
  Serial.println(action_str(general));
  Serial.print("Buzzer:");
  Serial.print("Message:");
  Serial.println(message);
}