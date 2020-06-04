#include "controler/ambiant.h"

#include "common/global.h"

bool AmbiantRule::proceed(Actions &actions) {
  if (ambiant_sensor.get_status() == WARNING) {
    actions.display   = warn;
    actions.octoprint = warn;
    actions.relay1    = warn;
    actions.relay2    = warn;
    actions.relay3    = warn;
    actions.relay4    = warn;

    if (ambiant_sensor.get_temperature_status() == WARNING)
      actions.message = "Ambiant temperature";
    else if (ambiant_sensor.get_humidity_status() == WARNING)
      actions.message = "Ambiant humidity";
  }
  return false;
}