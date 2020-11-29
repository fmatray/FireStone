#include "controler/aq.h"

#include "common/global.h"

bool AirQualityRule::proceed(Dispatcher &dispatcher) {
  if (air_quality.get_status() == ALERT) {
    if (air_quality.get_smoke_status() == ALERT)  // smoke
      dispatcher.set(alert, nothing, "Smoke");
    else if (air_quality.get_hcho_status() == ALERT)  // Formaldehyde
      dispatcher.set(alert, alert, "High HCHO");
    else  // Air Quality
      dispatcher.set(alert, alert, "Air Quality");
    return true;
  }

  if (air_quality.get_status() == WARNING) {
    if (air_quality.get_smoke_status() == WARNING)  // smoke
      dispatcher.set(warn, warn, "Smoke");
    else  // Air Quality
      dispatcher.set(warn, warn, "Air Quality");
    return true;
  }
  return false;
}