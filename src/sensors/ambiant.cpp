#include "sensors/ambiant.h"

#include <dhtnew.h>

#include "common/helpers.h"

/*
   SETUP
*/
AmbiantSensor::AmbiantSensor(const uint8_t _pin) {
  pin = _pin;
}

void AmbiantSensor::begin() {
  title("Ambiant Sensor Setup");
  TempHumiditySensor = new DHTNEW(pin);
}

void AmbiantSensor::update(const ambiant_settings_t _settings) {
  min_temp_ambiant  = _settings.min_temp_ambiant;
  max_temp_ambiant  = _settings.max_temp_ambiant;
  max_humid_ambiant = _settings.max_humid_ambiant;
  TempHumiditySensor->setTempOffset(_settings.temp_ambient_offset);
  TempHumiditySensor->setHumOffset(_settings.humid_ambient_offset);

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
  if (check_time(TempHumiditySensor->lastRead(), 5))  // read every 5s
  {
    title("Ambiant");
    if (read())
      status = check();
  }
}

bool AmbiantSensor::read() {
  int chk = TempHumiditySensor->read();

  if (chk == DHTLIB_ERROR_CHECKSUM || chk == DHTLIB_ERROR_TIMEOUT) {
    status = ERROR;
    return false;
  }
  temperature = TempHumiditySensor->getTemperature();
  humidity    = TempHumiditySensor->getHumidity();
  return true;
}

status_e AmbiantSensor::check() {
  temperature_status = check_temperature();
  humidity_status    = check_humidity();

  return max(temperature_status, humidity_status);
}

status_e AmbiantSensor::check_temperature() {
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C");

  if (temperature < 0) {
    Serial.println("(ERROR)");
    return ERROR;
  }

  if (min_temp_ambiant != NO_CHECK && temperature < min_temp_ambiant) {
    Serial.println("(WARNING)");
    return WARNING;
  }
  if (max_temp_ambiant != NO_CHECK && temperature > max_temp_ambiant) {
    Serial.println("(WARNING)");
    return WARNING;
  }
  Serial.println("(OK)");
  return OK;
}

status_e AmbiantSensor::check_humidity() {
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%");

  if (humidity < 0) {
    Serial.println("(ERROR)");
    return ERROR;
  }

  if (max_humid_ambiant != NO_CHECK && humidity > max_humid_ambiant) {
    Serial.println("(WARNING)");
    return WARNING;
  }
  Serial.println("(OK)");
  return OK;
}
