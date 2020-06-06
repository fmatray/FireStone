#include "controler/fire.h"

#include "common/global.h"

bool FireRule::proceed(Dispatcher &dispatcher) {
  if (fire_sensor.get_status() == ALERT) {
    dispatcher.set(alert, alert, "Fire");
    return true;
  }
  return false;
}