#ifndef SETTINGS_H
#define SETTINGS_H
#include "actions/relay.h"
#include "sensors/ambiant.h"
#include "sensors/octoprint.h"
#include "common/status.h"
/* Data */

class SettingsData {
 public:
  void populate(const SettingsData *obj);
  long calculate_checksum();
  /* Time */
  int8_t timezone_offset;  // TODO : use a TimeZone Library

  /* Ambiant */
  ambiant_settings_t ambiant_settings;
  /* Octoprint */
  octoprint_settings_t octoprint_settings;
  /* Relays */
  relay_settings_t relay1_settings;
  relay_settings_t relay2_settings;
  relay_settings_t relay3_settings;
  relay_settings_t relay4_settings;
  long checksum;
};

class Settings : public SettingsData {
 public:
  Settings();
  void reset();
  void begin();
  void load();
  void update();
  void save();
  void print();
  bool has_changed() { return _changed; };
  void changed() { _changed = true; };

 private:
  bool _changed;
};

#endif