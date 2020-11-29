#include "IO/aq.h"

#include "common/helpers.h"
#include "config.h"

void AirQuality::begin() {
  title("Air Quality Sensor Setup");
  pms = new SerialPM(PMS5003ST, Serial1);
  pms->init();
  pms->wake();
}

bool AirQuality::test() { return read(); }
bool AirQuality::read() {
  if (pms->read() == pms->OK)
    return true;
  status = ERROR;
  return false;
}

void AirQuality::update(const air_quality_settings_t _settings) {
  smoke_alert   = _settings.smoke_alert;
  smoke_warning = _settings.smoke_warning;
  smoke_pm25    = _settings.smoke_pm25;
  smoke_pm10    = _settings.smoke_pm10;
  max_hcho      = _settings.max_hcho;
}

status_e AirQuality::check() {
  if (pms->status != pms->OK)
    return ERROR;

  if (smoke >= smoke_warning + smoke_alert) {
    smoke_status = ALERT;
    Serial.println("Smoke ALERT");
    return ALERT;
  }
  if (smoke >= smoke_warning) {
    smoke_status = WARNING;
    Serial.println("Smoke WARNING");
    return WARNING;
  }
  if (pms->has_formaldehyde() && pms->hcho >= max_hcho) {
    hcho_status = ALERT;
    Serial.println("Formaldehyde ALERT");
    return ALERT;
  }
  smoke_status = OK;
  hcho_status  = OK;
  return OK;
}

void AirQuality::run() {
  static unsigned long lasttime = 500;

  if (!check_time(lasttime, 10))
    return;
  lasttime = millis();
  title("Air Quality");
  // read the PM sensor
  if (read()) {
    if (pms->pm25 > smoke_pm25 && pms->pm10 > smoke_pm10)
      smoke++;
    else
      smoke = 0;
    calculate_mean();
    status = check();
    print();
  } else  // something went wrong
    print_error();
}
void AirQuality::calculate_mean() {
  // 1h mean
  pm01_accumulation_1h += pms->pm01;
  pm25_accumulation_1h += pms->pm25;
  pm10_accumulation_1h += pms->pm10;
  nb_points_1h++;
  if (check_time(accumulation_1h_lasttime, 60 * 60)) {
    pm01_mean_1h         = pm01_accumulation_1h / nb_points_1h;
    pm25_mean_1h         = pm25_accumulation_1h / nb_points_1h;
    pm10_mean_1h         = pm10_accumulation_1h / nb_points_1h;
    CAQI_1h              = CAQI(pm25_mean_1h, pm10_mean_1h);
    pm01_accumulation_1h = 0;
    pm25_accumulation_1h = 0;
    pm10_accumulation_1h = 0;
    nb_points_1h         = 0;
  }
}

uint8_t AirQuality::CAQI(uint16_t pm25, uint16_t pm1O) {
  return max(CAQIPM25(pm25), CAQIPM10(pm1O));
}

const char *AirQuality::CAQI_str(uint16_t pm25, uint16_t pm1O) {
  uint8_t index = CAQI(pm25, pm1O);

  for (uint8_t i = 0; i < sizeof(CAQI_grid); i++)
    if (index == CAQI_grid[i].index)
      return CAQI_grid[i].str;
  return NULL;
}

uint8_t AirQuality::CAQIPM25(uint16_t value) {
  uint16_t last_PM25_max = 0;
  uint8_t i              = 0;

  for (i = 0; !(last_PM25_max <= value && value < CAQI_grid[i].PM25_max); i++)
    last_PM25_max = CAQI_grid[i].PM25_max;
  return CAQI_grid[i].index;
}

uint8_t AirQuality::CAQIPM10(uint16_t value) {
  uint16_t last_PM10_max = 0;
  uint8_t i              = 0;

  for (i = 0; !(last_PM10_max <= value && value < CAQI_grid[i].PM10_max); i++)
    last_PM10_max = CAQI_grid[i].PM10_max;
  return CAQI_grid[i].index;
}

void AirQuality::print_error() {
  switch (pms->status) {
    case pms->OK:  // should never come here
      break;       // included to compile without warnings
    case pms->ERROR_TIMEOUT:
      Serial.println(F(PMS_ERROR_TIMEOUT));
      break;
    case pms->ERROR_MSG_UNKNOWN:
      Serial.println(F(PMS_ERROR_MSG_UNKNOWN));
      break;
    case pms->ERROR_MSG_HEADER:
      Serial.println(F(PMS_ERROR_MSG_HEADER));
      break;
    case pms->ERROR_MSG_BODY:
      Serial.println(F(PMS_ERROR_MSG_BODY));
      break;
    case pms->ERROR_MSG_START:
      Serial.println(F(PMS_ERROR_MSG_START));
      break;
    case pms->ERROR_MSG_LENGTH:
      Serial.println(F(PMS_ERROR_MSG_LENGTH));
      break;
    case pms->ERROR_MSG_CKSUM:
      Serial.println(F(PMS_ERROR_MSG_CKSUM));
      break;
    case pms->ERROR_PMS_TYPE:
      Serial.println(F(PMS_ERROR_PMS_TYPE));
      break;
  }
}

void AirQuality::print() {
  // print the results
  Serial.print(F("PM1.0: "));
  Serial.print(pms->pm01);
  Serial.print(F("\tPM2.5: "));
  Serial.print(pms->pm25);
  Serial.print(F("\tPM10: "));
  Serial.print(pms->pm10);
  Serial.println(F(" [µg/m3]"));

  if (pms->has_number_concentration()) {
    Serial.print(F("N0.3: "));
    Serial.print(pms->n0p3);
    Serial.print(F("\tN0.5: "));
    Serial.print(pms->n0p5);
    Serial.print(F("\tN1.0: "));
    Serial.print(pms->n1p0);
    Serial.print(F("\tN2.5: "));
    Serial.print(pms->n2p5);
    Serial.print(F("\tN5.0: "));
    Serial.print(pms->n5p0);
    Serial.print(F("\tN10: "));
    Serial.print(pms->n10p0);
    Serial.println(F(" [#/l]"));
  }

  if (pms->has_temperature_humidity() || pms->has_formaldehyde()) {
    Serial.print(F("Temp: "));
    Serial.print(pms->temp);
    Serial.print(F(" °C"));
    Serial.print(F("\tHumidity: "));
    Serial.print(pms->rhum);
    Serial.print(F(" %rh"));
    Serial.print(F("\tFormaldehyde: "));
    Serial.print(pms->hcho);
    Serial.println(F(" mg/m3 HCHO"));
  }
  Serial.print("Smoke:");
  Serial.println(smoke);
  Serial.print("Air Quality:");
  Serial.println(CAQI_str(pms->pm25, pms->pm10));
}
