#include "controler/ambiant.h"

#include "common/global.h"

bool AmbiantRule::proceed(Dispatcher &dispatcher) {
  if (ambiant_sensor.get_status() == WARNING) {
    if (ambiant_sensor.get_temperature_status() == WARNING)
      dispatcher.set(warn, nothing, "Ambiant temperature");
    else if (ambiant_sensor.get_humidity_status() == WARNING)
      dispatcher.set(warn, nothing, "Ambiant humidity");
  }
  return false;
}