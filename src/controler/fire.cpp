#include "controler/fire.h"

#include "common/global.h"

bool FireRule::proceed(Actions &actions) {
  if (fire_sensor.get_status() == ALERT) {
    actions.display   = alert;
    actions.octoprint = alert;
    actions.buzzer    = alert;
    actions.relay1    = alert;
    actions.relay2    = alert;
    actions.relay3    = alert;
    actions.relay4    = alert;
    actions.message   = "Fire";
    return true;
  }
  return false;
}