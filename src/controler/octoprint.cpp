#include "controler/octoprint.h"

#include "common/global.h"

bool OctoprintRule::proceed(Dispatcher &dispatcher) {
  status_e status = octoprint.get_status();
  String message  = "";

  if (!octoprint.is_octoprint_connected() ||
      !octoprint.is_printer_operational()) return false;

  switch (status) {
    case ALERT:
      dispatcher.set(alert, alert, "Alert");
      break;
    case WARNING:
      dispatcher.set(warn, warn, "Warning");
      break;
    case ERROR:
      dispatcher.set(err, err, "Error");
      break;
    case OK:
    case IDLE:
    case OFF:
    case NO_CHECK:
      return false;
    default:
      Serial.println("Unkown status");
  }
  status_e status_tmp = octoprint.get_ext0_status();
  dispatcher.update_message("Extruder 0");

  if (octoprint.get_ext1_status() > status_tmp)
    dispatcher.update_message("Extruder 1");
  else if (octoprint.get_bed_status() > status_tmp)
    dispatcher.update_message("Bed");

  return true;
}