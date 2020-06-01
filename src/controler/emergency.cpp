#include "controler/emergency.h"

#include "global.h"

bool EmergencyRule::proceed(actions_t &actions) {
  if (emergency_sensor.get_status() == ALERT) {
    actions.global  = ALERT;
    actions.relay1  = ALERT;
    actions.relay2  = ALERT;
    actions.relay3  = ALERT;
    actions.relay4  = ALERT;
    actions.message = "Emergency";
    return true;
  }
  return false;
}