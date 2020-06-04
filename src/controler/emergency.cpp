#include "controler/emergency.h"

#include "common/global.h"

bool EmergencyRule::proceed(Actions &actions) {
  if (emergency_sensor.get_status() == ALERT) {
    actions.display = alert;
    actions.relay1  = alert;
    actions.relay2  = alert;
    actions.relay3  = alert;
    actions.relay4  = alert;
    actions.message = "Emergency";
    return true;
  }
  return false;
}