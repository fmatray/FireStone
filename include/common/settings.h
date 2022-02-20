#ifndef SETTINGS_H
#define SETTINGS_H
#include "IO/ambiant.h"
#include "IO/aq.h"
#include "IO/octoprint.h"
#include "IO/relay.h"
#include "IO/timer.h"
#include "common/status.h"
/* Data */

class SettingsData {
 public:
  void populate(const SettingsData *obj);
  long calculate_checksum();
  /* Time */
  int8_t timezone_offset;  // TODO : use a TimeZone Library
  timer_settings_t timer_settings;
  /* air_quality */
  air_quality_settings_t air_quality_settings;
  /* Ambiant */
  ambiant_settings_t ambiant_settings;
  /* Octoprint */
  octoprint_settings_t octoprint_settings;
  /* Relays */
  /* !!!!!! DISABLED FOR NOW
  relay_settings_t relay1_settings;
  relay_settings_t relay2_settings;
  relay_settings_t relay3_settings;
  relay_settings_t relay4_settings;
  */
  long checksum;
};

class Settings : public SettingsData {
 public:
  Settings();
  void reset();
  bool begin();
  bool load();
  void update();
  void save();
  void print();
  bool has_changed() { return _changed; };
  void changed() { _changed = true; };

 private:
  bool _changed;
};

#endif