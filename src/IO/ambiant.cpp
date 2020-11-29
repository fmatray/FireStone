#include "IO/ambiant.h"

#include <dhtnew.h>

#include "common/helpers.h"

/*
   SETUP
*/
void AmbiantSensor::begin() {
  title("Ambiant Sensor Setup");
#ifdef USE_DHT
  TempHumiditySensor = new DHTNEW(TEMP_HUMIDITY_PIN);
#endif
}

bool AmbiantSensor::test() {
#ifdef USE_DHT
  return TempHumiditySensor->read() == DHTLIB_OK;
#else
  return air_quality.test();
  return true;
#endif
};

void AmbiantSensor::update(const ambiant_settings_t _settings) {
  min_temp_ambiant  = _settings.min_temp_ambiant;
  max_temp_ambiant  = _settings.max_temp_ambiant;
  max_humid_ambiant = _settings.max_humid_ambiant;
#ifdef USE_DHT
  TempHumiditySensor->setTempOffset(_settings.temp_ambient_offset);
  TempHumiditySensor->setHumOffset(_settings.humid_ambient_offset);
#else
  air_quality.set_temp_offset(_settings.temp_ambient_offset);
  air_quality.set_rhum_offset(_settings.humid_ambient_offset);
#endif
  if (read())
    status = check();
}

void AmbiantSensor::reset() {
  status             = OK;
  temperature_status = OK;
  humidity_status    = OK;
}
/*
   LOOP
*/
void AmbiantSensor::run() {
  static unsigned long lasttime = 0;

  if (!check_time(lasttime, 5))
    return;
  lasttime = millis();
  title("Ambiant");
  if (read())
    status = check();
}

bool AmbiantSensor::read() {
#ifdef USE_DHT
  int chk = TempHumiditySensor->read();

  if (chk == DHTLIB_ERROR_CHECKSUM || chk == DHTLIB_ERROR_TIMEOUT) {
    status = ERROR;
    return false;
  }
  temperature = TempHumiditySensor->getTemperature();
  humidity    = TempHumiditySensor->getHumidity();
#else
  if (air_quality.get_status() == ERROR) {
    status = ERROR;
    return false;
  }
  temperature = air_quality.temp();
  humidity    = air_quality.rhum();
#endif
  return true;
}

status_e AmbiantSensor::check() {
  static uint8_t non_ok = 0;

  temperature_status = check_temperature();
  humidity_status    = check_humidity();

  if (temperature_status > OK || humidity_status > OK)
    non_ok++;
  else
    non_ok = 0;
  if (non_ok > 3)
    return max(temperature_status, humidity_status);
  return OK;
}

status_e AmbiantSensor::check_temperature() {
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C");

  if (temperature < 0) {
    Serial.println(" (ERROR)");
    return ERROR;
  }

  if (min_temp_ambiant != NO_CHECK && temperature < min_temp_ambiant) {
    Serial.println(" (WARNING)");
    return WARNING;
  }
  if (max_temp_ambiant != NO_CHECK && temperature > max_temp_ambiant) {
    Serial.println(" (WARNING)");
    return WARNING;
  }
  Serial.println(" (OK)");
  return OK;
}

status_e AmbiantSensor::check_humidity() {
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%");

  if (humidity < 0) {
    Serial.println(" (ERROR)");
    return ERROR;
  }

  if (max_humid_ambiant != NO_CHECK && humidity > max_humid_ambiant) {
    Serial.println(" (WARNING)");
    return WARNING;
  }
  Serial.println(" (OK)");
  return OK;
}
