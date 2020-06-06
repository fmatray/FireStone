#include "controler/ambiant.h"

#include "common/global.h"

bool AmbiantRule::proceed(Actions &actions) {
  if (ambiant_sensor.get_status() == WARNING) {
    actions.display_act   = warn;
    actions.octoprint_act = warn;
    actions.relay1_act    = warn;
    actions.relay2_act    = warn;
    actions.relay3_act    = warn;
    actions.relay4_act    = warn;

    if (ambiant_sensor.get_temperature_status() == WARNING)
      actions.message = "Ambiant temperature";
    else if (ambiant_sensor.get_humidity_status() == WARNING)
      actions.message = "Ambiant humidity";
  }
  return false;
}