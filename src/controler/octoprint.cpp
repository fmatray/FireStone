#include "controler/octoprint.h"

#include "common/global.h"

bool OctoprintRule::proceed(actions_t &actions) {
  status_e status = octoprint_sensor.get_status();

  switch (status) {
    case ALERT:
      actions.relay1 = ALERT;
      actions.relay2 = ALERT;
      actions.relay3 = ALERT;
      actions.relay4 = ALERT;
    case WARNING:
      actions.buzzer = status;
    case ERROR:
      actions.global  = octoprint_sensor.get_ext0_status();
      actions.message = "Extruder 0";

      if (octoprint_sensor.get_ext1_status() > actions.global) {
        actions.global  = octoprint_sensor.get_ext1_status();
        actions.message = "Extruder 1";
      }
      if (octoprint_sensor.get_bed_status() > actions.global) {
        actions.global  = octoprint_sensor.get_bed_status();
        actions.message = "Bed";
      }
      return true;
    case OK:
    case IDLE:
    case OFF:
    case NO_CHECK:
      return false;
    default:
      Serial.println("Unkown status");
  }
  return false;
}