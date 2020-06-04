#include "controler/ambiant.h"

#include "common/global.h"

bool AmbiantRule::proceed(actions_t &actions) {
  if (ambiant_sensor.get_status() == WARNING) {
    actions.global = WARNING;
    actions.relay1 = WARNING;
    actions.relay2 = WARNING;
    actions.relay3 = WARNING;
    actions.relay4 = WARNING;

    if (ambiant_sensor.get_temperature_status() == WARNING)
      actions.message = "Ambiant temperature";
    else if (ambiant_sensor.get_humidity_status() == WARNING)
      actions.message = "Ambiant humidity";
  }
  return false;
}