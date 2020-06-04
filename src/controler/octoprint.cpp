#include "controler/octoprint.h"

#include "common/global.h"

bool OctoprintRule::proceed(Actions &actions) {
  status_e status = octoprint_sensor.get_status();

  switch (status) {
    case ALERT:
      actions = {alert,         // display
                 alert, alert,  // Buzzer, Octoprint
                 alert, alert,  // Relay 1, 2,
                 alert, alert,  // Relay 3,4
                 "Alert"};      // Message
      break;
    case WARNING:
      actions = {warn,        // display
                 warn, warn,  // Buzzer, Octoprint
                 warn, warn,  // Relay 1, 2,
                 warn, warn,  // Relay 3,4
                 ""};         // Message
      break;
    case ERROR:
      actions = {err,       // display
                 err, err,  // Buzzer, Octoprint
                 err, err,  // Relay 1, 2,
                 err, err,  // Relay 3,4
                 ""};       // Message
      break;
    case OK:
    case IDLE:
    case OFF:
    case NO_CHECK:
      return false;
    default:
      Serial.println("Unkown status");
  }
  status_e status_tmp = octoprint_sensor.get_ext0_status();
  actions.message     = "Extruder 0";

  if (octoprint_sensor.get_ext1_status() > status_tmp)
    actions.message = "Extruder 1";
  else if (octoprint_sensor.get_bed_status() > status_tmp)
    actions.message = "Bed";

  return true;
}