#ifndef AMBIANT_H
#define AMBIANT_H

#include <dhtnew.h>

#include "sensor.h"
typedef struct {
  float temp_ambient_offset;   // Adjust temperature offset
  float humid_ambient_offset;  // Adjust temperature offset
  uint8_t min_temp_ambiant;    // Min ambiant temparature
  uint8_t max_temp_ambiant;    // Max ambiant temparature
  uint8_t max_humid_ambiant;   // MAX ambiant humidity
} ambiant_settings_t;

class AmbiantSensor : public Sensor {
 public:
  AmbiantSensor(const uint8_t _pin);
  void begin();
  void update(const ambiant_settings_t _settings);
  bool test() { return TempHumiditySensor->read() == DHTLIB_OK; };
  void reset(); 
  void run();
  bool read();

  status_e check();
  status_e check_temperature();
  status_e check_humidity();
  status_e get_temperature_status() { return temperature_status; }
  status_e get_humidity_status() { return humidity_status; }

  int get_temperature() { return temperature; };
  int get_humidity() { return humidity; };

 private:
  DHTNEW *TempHumiditySensor;
  uint8_t pin;

  float temperature           = 0;
  status_e temperature_status = NO_CHECK;
  float humidity              = 0;
  status_e humidity_status    = NO_CHECK;

  uint8_t min_temp_ambiant  = 0;  // min ambiant temparature
  uint8_t max_temp_ambiant  = 0;  // max ambiant temparature
  uint8_t max_humid_ambiant = 0;  // max ambiant humidity
};

#endif
