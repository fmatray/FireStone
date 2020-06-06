#include "controler/emergency.h"

#include "common/global.h"

bool EmergencyRule::proceed(Actions &actions) {
  if (emergency_sensor.get_status() == ALERT) {
    actions.display_act   = alert;
    actions.octoprint_act = alert;
    actions.relay1_act    = alert;
    actions.relay2_act    = alert;
    actions.relay3_act    = alert;
    actions.relay4_act    = alert;
    actions.message       = "Emergency";
    return true;
  }
  return false;
}