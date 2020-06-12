#include "IO/octoprint.h"

#include <OctoPrintAPI.h>
#include <WiFiNINA.h>

#include "common/debug.h"
#include "common/helpers.h"
#include "config.h"

void OctoPrint::begin() {
  api = new OctoprintApi(client, OCTOPRINT_IP, OCTOPRINT_PORT, OCTOPRINT_APIKEY);
#ifdef FS_DEBUG
  api->_debug = true;
#endif
}

void OctoPrint::update(const octoprint_settings_t _settings) {
  max_temp_ext0 = _settings.max_temp_ext0;
  max_temp_ext1 = _settings.max_temp_ext1;
  max_temp_bed  = _settings.max_temp_bed;
  interval      = _settings.interval;
}

void OctoPrint::reset() {
  status      = OK;
  ext0_status = OK;
  ext1_status = OK;
  bed_status  = OK;
  action_sent = false;
}
/*
    RUN
*/

void OctoPrint::run_sensor() {
  static unsigned long printer_lasttime           = 0;
  static unsigned long job_lasttime               = 1000;
  static unsigned long octoprint_version_lasttime = 10000;

  if (check_time(printer_lasttime, interval)) {  //Check if time has expired to go check OctoPrint Status
    if (read())
      status = check();
    printer_lasttime = millis();
  } else if (printer_working && check_time(job_lasttime, 20)) {
    read_job();
    job_lasttime = millis();
  } else if (check_time(octoprint_version_lasttime, 3600)) {  //Check if time has expired to go check OctoPrint Version
    check_version();
    octoprint_version_lasttime = millis();
  }
}

bool OctoPrint::read() {
  DEBUG1("Start reading");
  octoprint_status    = not_connected;
  bool ret            = api->getPrinterStatistics();
  printer_state       = api->printerStats.printerState;
  printer_operational = false;
  printer_status      = unknow;
  printer_working     = false;
  ext0_available      = false;
  ext1_available      = false;
  bed_available       = false;
  ext0_temp           = 0;
  ext1_temp           = 0;
  bed_temp            = 0;

  DEBUG1("Answer");
  if (!ret || printer_state == "Printer is not operational") {  // Reset
    if (!ret) {
      printer_state  = "Cannot connect to OctoPrint";
      printer_status = closed;
      status         = ERROR;
      DEBUG1(printer_state);
      return false;
    }
    status           = IDLE;
    octoprint_status = connected;
    return false;
  }
  // Status
  octoprint_status    = connected;
  printer_operational = api->printerStats.printerStateoperational;

  if (api->printerStats.printerStateclosedOrError)
    printer_status = api->printerStats.printerStateerror ? error : closed;
  else if (api->printerStats.printerStateready)
    printer_status = ready;
  else if (api->printerStats.printerStatePrinting) {
    printer_status  = printing;
    printer_working = true;
  } else if (api->printerStats.printerStatepaused) {
    printer_status  = paused;
    printer_working = true;
  } else if (api->printerStats.printerStatepausing) {
    printer_status  = pausing;
    printer_working = true;
  } else if (api->printerStats.printerStateresuming) {
    printer_status  = resuming;
    printer_working = true;
  } else if (api->printerStats.printerStatefinishing) {
    printer_status  = finishing;
    printer_working = true;
  } else
    printer_status = unknow;

  // Sensors
  if (api->printerStats.printerTool0Available) {
    ext0_available = true;
    ext0_temp      = api->printerStats.printerTool0TempActual;
    ext0_target    = api->printerStats.printerTool0TempTarget;
    ext0_offset    = api->printerStats.printerTool0TempOffset;
  }
  if (api->printerStats.printerTool1Available) {
    ext1_available = true;
    ext1_temp      = api->printerStats.printerTool1TempActual;
    ext1_target    = api->printerStats.printerTool1TempTarget;
    ext1_offset    = api->printerStats.printerTool1TempOffset;
  }
  if (api->printerStats.printerBedAvailable) {
    bed_available = true;
    bed_temp      = api->printerStats.printerBedTempActual;
    bed_target    = api->printerStats.printerBedTempTarget;
    bed_offset    = api->printerStats.printerBedTempOffset;
  }
  DEBUG1(printer_state);
  return true;
}
/*
   OCTOPRINT checks
*/
status_e OctoPrint::check() {
  ext0_status = (max_temp_ext0 == NO_CHECK) ? NO_CHECK : OFF;
  ext1_status = (max_temp_ext1 == NO_CHECK) ? NO_CHECK : OFF;
  bed_status  = (max_temp_bed == NO_CHECK) ? NO_CHECK : OFF;

  title("OctoPrint Status");
  Serial.print("Printer Current State: ");
  Serial.println(printer_state);

  if (!printer_operational)
    return OFF;
  if (max_temp_ext0)
    ext0_status = check_temperature("Extruder0: ", ext0_temp, max_temp_ext0, ext0_target);
  if (max_temp_ext1)
    ext1_status = check_temperature("Extruder1: ", ext1_temp, max_temp_ext1, ext1_target);
  if (max_temp_bed)
    bed_status = check_temperature("Bed: ", bed_temp, max_temp_bed, bed_target);
  Serial.println("");
  return max(max(ext0_status, ext1_status), bed_status);
}

status_e OctoPrint::check_temperature(const char *item, const float temp, const float max_temp, const float target) {
  Serial.print(item);
  Serial.print(temp);
  Serial.print("°C/");
  Serial.print(target);
  Serial.print("°C ");

  if (max_temp == NO_CHECK) {
    Serial.print("(No check)\t");
    return OFF;
  }

  if (temp > max_temp) {
    Serial.print("(ALERT)\t");
    return ALERT;
  }
  if (temp > max_temp * 0.95) {  // WARNING at 95% of  max temperature
    Serial.print("(WARNING)\t");
    return WARNING;
  }

  if (temp < 0) {
    Serial.print("(Error)\t");
    return ERROR;
  }
  if (temp == 0) {
    Serial.print("(Off)\t");
    return OFF;
  }
  Serial.print("(OK)\t");
  return OK;
}

status_e OctoPrint::check_version() {
  if (!wificonnexion.is_connected())
    return ERROR;
  DEBUG1("Start check version");
  if (api->getOctoprintVersion()) {
    title("OctoPrint Version");

    Serial.print("Octoprint API: ");
    api_version = api->octoprintVer.octoprintApi;
    Serial.println(api_version);

    Serial.print("Octoprint Server: ");
    octoprint_version = api->octoprintVer.octoprintServer;
    Serial.println(octoprint_version);
    return OK;
  }
  DEBUG1("Error check version");
  return ERROR;
}

String OctoPrint::get_printer_status_as_str() {
  switch (printer_status) {
    case closed:
      return "Closed";
    case error:
      return "Error";
    case ready:
      return "Ready";
    case printing:
      return "Printing";
    case paused:
      return "Paused";
    case pausing:
      return "Pausing";
    case resuming:
      return "Resuming";
    case finishing:
      return "Finishing";
    case unknow:
      return "Unknow";
  }
  return "UNKOWN STATUS";
}

void OctoPrint::read_job() {
  DEBUG1("Start read job");
  bool ret = api->getPrintJob();

  completion = ret ? api->printJob.progressCompletion : 0;
  timer.clear();
  DEBUG1("End read job");
}

void OctoPrint::run_action() {
  if (!wificonnexion.is_connected() || action_sent)
    return;
  Serial.print("Octoprint action:");
  Serial.println(action_str(action));
  action_sent = true;
  return;  // TO REMOVE AFTER DEBUGGING
  switch (action) {
    case nothing:
      return;
    case off:
      api->octoPrintCoolDown();
      api->octoPrintConnectionDisconnect();
      delay(1000);
      break;
    case sleep:
      api->octoPrintCoolDown();
      break;
    case wakeup:
      api->octoPrintConnectionAutoConnect();
      break;
    case err:
      return;
    case warn:
      return;
    case alert:
      api->octoPrintJobCancel();  // Cancel current job
      api->octoPrintCoolDown();
      api->octoPrintPrintHeadRelativeJog(0, 0, 50, 1000);  // fast head lifting
      delay(2000);
      api->octoPrintPrinterCommand("M112");  // Emergency STOP
      break;
    default:
      break;
  }
  action_sent = true;
}