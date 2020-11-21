#ifndef AQ_H
#define AQ_H

#include <Arduino.h>
#include <PMserial.h>
#include "sensor.h"

typedef struct {
  uint8_t smoke_warning;  // nb reading thresholded before warning
  uint8_t smoke_alert;    // nb warning reading before atler
  uint16_t smoke_pm25;    // PM2.5 threshold
  uint16_t smoke_pm10;    // PM10 threshold
} air_quality_settings_t;

const struct {
  uint8_t index;
  uint16_t PM25_max;
  uint16_t PM10_max;
  const char *str;
} CAQI_grid[] = {
    {0, 15, 25, "Good"},
    {25, 30, 50, "Acceptable"},
    {50, 55, 90, "Moderate"},
    {75, 110, 180, "Heavy"},
    {100, 65535, 65535, "Severe"},
};

class AirQuality : public Sensor {
 public:
  void begin();
  bool test();
  void run();
  bool read();
  void update(const air_quality_settings_t _settings);
  status_e check();

  uint16_t pm01() { return pms->pm01; };  // µg/m3
  uint16_t pm25() { return pms->pm25; };  // µg/m3 limit: 15µg/m3
  uint16_t pm10() { return pms->pm10; };  // µg/m3 limit : 30µg/m3

  uint16_t n0p3() { return pms->n0p3 * 10; };    // number of 0.3 particules per L
  uint16_t n0p5() { return pms->n0p5 * 10; };    // number of 0.5 particules per L
  uint16_t n1p0() { return pms->n1p0 * 10; };    // number of 1.0 particules per L
  uint16_t n2p5() { return pms->n2p5 * 10; };    // number of 2.5 particules per L
  uint16_t n5p0() { return pms->n5p0 * 10; };    // number of 5.0 particules per L
  uint16_t n10p0() { return pms->n10p0 * 10; };  // number of 10.0 particules per L

  float temp() { return pms->temp; };  // °C
  float rhum() { return pms->rhum; };  // %
  float hcho() { return pms->hcho; };  // mg/m3

 private:
  SerialPM *pms;

  // Settings
  uint8_t smoke_warning;
  uint8_t smoke_alert;
  uint16_t smoke_pm25;
  uint16_t smoke_pm10;
  uint16_t smoke = 0;

  // 1h mean
  uint16_t pm01_mean_1h = 0;
  uint16_t pm25_mean_1h = 0;
  uint16_t pm10_mean_1h = 0;
  uint8_t CAQI_1h       = 0;

  uint16_t pm01_accumulation_1h     = 0;
  uint16_t pm25_accumulation_1h     = 0;
  uint16_t pm10_accumulation_1h     = 0;
  uint16_t nb_points_1h             = 0;
  uint32_t accumulation_1h_lasttime = 0;
  void calculate_mean();

  void print_error();
  void print();

  uint8_t CAQI(uint16_t pm25, uint16_t pm1O);
  const char *CAQI_str(uint16_t pm25, uint16_t pm1O);
  uint8_t CAQIPM25(uint16_t value);
  uint8_t CAQIPM10(uint16_t value);
};

#endif