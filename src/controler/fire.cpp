#include "controler/fire.h"

#include "global.h"

bool FireRule::proceed(actions_t &actions) {
  if (fire_sensor.get_status() == ALERT) {
    actions.global  = ALERT;
    actions.buzzer  = ALERT;
    actions.relay1  = ALERT;
    actions.relay2  = ALERT;
    actions.relay3  = ALERT;
    actions.relay4  = ALERT;
    actions.message = "Fire";
    return true;
  }
  return false;
}