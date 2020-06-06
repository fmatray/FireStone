#include "controler/actions.h"

#include <Arduino.h>

#include "common/helpers.h"

bool Actions::operator!=(const Actions &a) {
  return display_act != a.display_act ||
         buzzer_act != a.buzzer_act ||
         octoprint_act != a.octoprint_act ||
         relay1_act != a.relay1_act || relay2_act != a.relay2_act ||
         relay3_act != a.relay3_act || relay4_act != a.relay4_act;
};

void Actions::reset() {
  display_act   = off;
  buzzer_act    = off;
  octoprint_act = off;
  relay1_act    = off;
  relay2_act    = off;
  relay3_act    = off;
  relay4_act    = off;
}

bool Actions::need_reset() {
  return display_act >= err ||
         buzzer_act >= err ||
         octoprint_act >= err ||
         relay1_act >= err || relay2_act >= err ||
         relay3_act >= err || relay4_act >= err;
};

void Actions::dispatch() {
  display.set(display_act, message);
  buzzer.set(buzzer_act);
  octoprint.set(octoprint_act);
  relay1.set(relay1_act);
  relay2.set(relay2_act);
  relay3.set(relay3_act);
  relay4.set(relay4_act);
}

void Actions::print() {
  title("Actions");
  Serial.print("Display:");
  Serial.println(action_str(display_act));
  Serial.print("Buzzer:");
  Serial.println(action_str(buzzer_act));
  Serial.print("Relay1:");
  Serial.println(action_str(relay1_act));
  Serial.print("Relay2:");
  Serial.println(action_str(relay2_act));
  Serial.print("Relay3:");
  Serial.println(action_str(relay3_act));
  Serial.print("Relay4:");
  Serial.println(action_str(relay4_act));
  Serial.print("Message:");
  Serial.println(message);
}