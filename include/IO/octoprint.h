#ifndef OCTOPRINT_H
#define OCTOPRINT_H
#include <Arduino.h>
#include <OctoPrintAPI.h>
#include <WiFiNINA.h>

#include "IO/action.h"
#include "IO/sensor.h"
#include "config.h"

typedef struct {
  uint8_t interval;        // INTERVAL between requests
  uint16_t max_temp_ext0;  // Max temparature for Extruder0
  uint16_t max_temp_ext1;  // Max temparature for Extruder1
  uint16_t max_temp_bed;   // Max temparature for the Bed
} octoprint_settings_t;

enum octoprint_status_e {
  not_connected,
  connected
};

enum printer_status_e {
  closed,
  error,
  ready,
  printing,
  paused,
  pausing,
  resuming,
  finishing,
  unknow
};

class OctoPrint : public Sensor, public Action {
 public:
  void begin();
  void update(const octoprint_settings_t _settings);

  bool test() { return check_version() == OK; };
  void reset();
  void run(){};
  void run_sensor();
  void run_action();
  bool read();
  status_e check();

  /* Status octoprint */
  bool is_octoprint_connected() { return octoprint_status == connected; };
  octoprint_status_e get_octoprint_status() { return octoprint_status; };
  const String get_octoprint_status_as_str() { return octoprint_status == connected ? "Connected" : "Not connected"; };
  String get_api_version() { return api_version; };
  String get_octoprint_version() { return octoprint_version; };

  /* Status printer */
  bool is_printer_operational() { return printer_operational; };
  bool is_working() { return printer_working; };
  String get_printer_state() { return printer_state; };
  printer_status_e get_printer_status() { return printer_status; };
  String get_printer_status_as_str();

  /* Job status */
  uint8_t get_completion() { return completion; };

  /* Tools */
  bool is_ext0_available() { return (printer_operational && ext0_available); };
  void set_ext0(float actual, float target) {
    ext0_temp   = actual;
    ext0_target = target;
    ext0_status = check_temperature("Extruder0: ", ext0_temp, max_temp_ext0, ext0_target);
  };
  float get_ext0_temp() { return ext0_temp; };
  float get_ext0_target() { return ext0_target; };
  float get_ext0_offset() { return ext0_offset; };

  bool is_ext1_available() { return (printer_operational && ext1_available); };
  void set_ext1(float actual, float target) {
    ext1_temp   = actual;
    ext1_target = target;
    ext1_status = check_temperature("Extruder1: ", ext1_temp, max_temp_ext1, ext1_target);
  };

  float get_ext1_temp() { return ext1_temp; };
  float get_ext1_target() { return ext1_target; };
  float get_ext1_offset() { return ext1_offset; };

  bool is_bed_available() { return (printer_operational && bed_available); };
  void set_bed(float actual, float target) {
    bed_temp   = actual;
    bed_target = target;
    bed_status = check_temperature("Bed: ", bed_temp, max_temp_bed, bed_target);
  };

  float get_bed_temp() { return bed_temp; };
  float get_bed_target() { return bed_target; };
  float get_bed_offset() { return bed_offset; };

  status_e get_ext0_status() { return ext0_status; };
  status_e get_ext1_status() { return ext1_status; };
  status_e get_bed_status() { return bed_status; };

 private:
  status_e check_temperature(const char *item, const float temp, const float max_temp, const float target);
  status_e check_version();
  void read_job();

  OctoprintApi *api;
  WiFiClient client;
  uint8_t interval;
  bool action_sent = false;

  /* Version */
  String api_version;
  String octoprint_version;

  /* Status octoprint */
  octoprint_status_e octoprint_status = not_connected;

  /* Status printer */
  String printer_state;
  bool printer_operational        = false;
  bool printer_working            = false;
  printer_status_e printer_status = unknow;

  /* Job status */
  uint8_t completion;

  /* Temperatures */
  bool ext0_available = false;
  float ext0_temp     = 0;
  float ext0_target   = 0;
  float ext0_offset   = 0;
  status_e ext0_status;

  bool ext1_available = false;
  float ext1_temp     = 0;
  float ext1_target   = 0;
  float ext1_offset   = 0;
  status_e ext1_status;

  bool bed_available = false;
  float bed_temp     = 0;
  float bed_target   = 0;
  float bed_offset   = 0;
  status_e bed_status;

  uint16_t max_temp_ext0;
  uint16_t max_temp_ext1;
  uint16_t max_temp_bed;
};

#endif