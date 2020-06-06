#include "controler/fire.h"

#include "common/global.h"

bool FireRule::proceed(Actions &actions) {
  if (fire_sensor.get_status() == ALERT) {
    actions.display_act   = alert;
    actions.octoprint_act = alert;
    actions.buzzer_act    = alert;
    actions.relay1_act    = alert;
    actions.relay2_act    = alert;
    actions.relay3_act    = alert;
    actions.relay4_act    = alert;
    actions.message       = "Fire";
    return true;
  }
  return false;
}