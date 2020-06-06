#include "controler/emergency.h"

#include "common/global.h"

bool EmergencyRule::proceed(Dispatcher &dispatcher) {
  if (emergency_sensor.get_status() == ALERT) {
    dispatcher.set(alert, alert, "Emergency");
    return true;
  }
  return false;
}