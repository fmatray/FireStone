#include "actions/octoprint.h"

#include <OctoPrintAPI.h>
#include <WiFiNINA.h>

#include "common/debug.h"
#include "common/helpers.h"
#include "config.h"

void OctoPrintAction::run() {
  if (WiFi.status() != WL_CONNECTED)
    return;
  switch (action) {
    case off:
      break;
    case err:
      break;
    case warn:
      break;
    case alert:
      if (alert_sent)
        return;
      api->octoPrintJobCancel();             // Cancel current job
      api->octoPrintSetTool0Temperature(0);  // start cooling
      api->octoPrintSetTool1Temperature(0);
      api->octoPrintSetBedTemperature(0);
      api->octoPrintPrintHeadRelativeJog(0, 0, 50, 1000);  // fast head lifting
      delay(2000);
      api->octoPrintPrinterCommand("M112");  // Emergency STOP
      alert_sent = true;
      break;
    default:
      break;
  }
}