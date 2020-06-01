#include "settings.h"

#include <FlashStorage.h>

#include "global.h"
#include "helpers.h"

void SettingsData::populate(const SettingsData *obj) {
  timezone_offset = obj->timezone_offset;
  /* Ambiant */
  ambiant_settings.temp_ambient_offset  = obj->ambiant_settings.temp_ambient_offset;
  ambiant_settings.humid_ambient_offset = obj->ambiant_settings.humid_ambient_offset;
  ambiant_settings.min_temp_ambiant     = obj->ambiant_settings.min_temp_ambiant;
  ambiant_settings.max_temp_ambiant     = obj->ambiant_settings.max_temp_ambiant;
  ambiant_settings.max_humid_ambiant    = obj->ambiant_settings.max_humid_ambiant;

  /* Octoprint */
  octoprint_settings.interval      = obj->octoprint_settings.interval;
  octoprint_settings.max_temp_ext0 = obj->octoprint_settings.max_temp_ext0;
  octoprint_settings.max_temp_ext1 = obj->octoprint_settings.max_temp_ext1;
  octoprint_settings.max_temp_bed  = obj->octoprint_settings.max_temp_bed;

  /* Relays */
  relay1_settings = obj->relay1_settings;
  relay2_settings = obj->relay2_settings;
  relay3_settings = obj->relay3_settings;
  relay4_settings = obj->relay4_settings;
  checksum        = this->calculate_checksum();
  Serial.print("Checksum:");
  Serial.println(checksum);
}
long SettingsData::calculate_checksum() {  // not a real checksum but enought to validate our datas
  return (timezone_offset + (ambiant_settings.temp_ambient_offset + ambiant_settings.humid_ambient_offset) +
          (ambiant_settings.min_temp_ambiant + ambiant_settings.max_temp_ambiant) +
          (octoprint_settings.interval + octoprint_settings.max_temp_ext0 + octoprint_settings.max_temp_ext1 + octoprint_settings.max_temp_bed)) +
         1984;
}

/* Settings */
Settings::Settings() { reset(); }

void Settings::reset() {
  timezone_offset = +2;
  /* Ambiant */
  ambiant_settings.temp_ambient_offset  = 0;
  ambiant_settings.humid_ambient_offset = 0;
  ambiant_settings.min_temp_ambiant     = 15;
  ambiant_settings.max_temp_ambiant     = 40;
  ambiant_settings.max_humid_ambiant    = 60;

  /* Octoprint */
  octoprint_settings.interval      = 10;
  octoprint_settings.max_temp_ext0 = 275;
  octoprint_settings.max_temp_ext1 = 275;
  octoprint_settings.max_temp_bed  = 120;

  /* Relays */
  relay1_settings.inverted      = false;
  relay1_settings.switch_status = ALERT;
  relay2_settings.inverted      = false;
  relay2_settings.switch_status = ALERT;
  relay3_settings.inverted      = false;
  relay3_settings.switch_status = ALERT;
  relay4_settings.inverted      = false;
  relay4_settings.switch_status = ALERT;

  _changed = false;
}
FlashStorage(settings_store, SettingsData);

void Settings::begin() { load(); }
void Settings::load() {
  SettingsData saved_data = settings_store.read();
  title("Loading Settings");
  if (saved_data.checksum == saved_data.calculate_checksum()) {
    this->populate(&saved_data);
    Serial.println("Success");
  } else
    Serial.println("Fail");
}

void Settings::update() {
  print();
  ambiant_sensor.update(ambiant_settings);
  octoprint_sensor.update(octoprint_settings);

  relay1.update(relay1_settings);
  relay2.update(relay2_settings);
  relay3.update(relay3_settings);
  relay4.update(relay4_settings);
  _changed = false;
}

void Settings::save() {
  SettingsData data;

  data.populate((SettingsData *)this);
  settings_store.write(data);
  title("Settings saved");
}
void Settings::print() {
  title("Settings");
  Serial.print("Timezone offset:");
  Serial.print(timezone_offset);
  Serial.println("h");

  Serial.print("Temp ambient offset:");
  Serial.print(ambiant_settings.temp_ambient_offset);
  Serial.println("°C");
  Serial.print("Humid_ambient offset:");
  Serial.print(ambiant_settings.humid_ambient_offset);
  Serial.println("%");
  Serial.print("Min temp ambiant:");
  Serial.print(ambiant_settings.min_temp_ambiant);
  Serial.println("°C");
  Serial.print("Max temp ambiant:");
  Serial.print(ambiant_settings.max_temp_ambiant);
  Serial.println("°C");
  Serial.print("Max humid ambiant:");
  Serial.print(ambiant_settings.max_humid_ambiant);
  Serial.println("%");

  Serial.print("Octroprint Interval:");
  Serial.print(octoprint_settings.interval);
  Serial.println("s");
  Serial.print("Max temp ext0:");
  Serial.print(octoprint_settings.max_temp_ext0);
  Serial.println("°C");
  Serial.print("Max temp ext1:");
  Serial.print(octoprint_settings.max_temp_ext1);
  Serial.println("°C");
  Serial.print("Max temp bed:");
  Serial.print(octoprint_settings.max_temp_bed);
  Serial.println("°C");

  Serial.print("Relay1:");
  Serial.print((relay1_settings.inverted ? "" : "not "));
  Serial.print("inverted ");
  Serial.println("Switch on:" + status_str(relay1_settings.switch_status));
  Serial.print("Relay2:");
  Serial.print((relay2_settings.inverted ? "" : "not "));
  Serial.print("inverted ");
  Serial.println("Switch on:" + status_str(relay1_settings.switch_status));
  Serial.print("Relay3:");
  Serial.print((relay2_settings.inverted ? "" : "not "));
  Serial.print("inverted ");
  Serial.println("Switch on:" + status_str(relay1_settings.switch_status));
  Serial.print("Relay4:");
  Serial.print((relay2_settings.inverted ? "" : "not "));
  Serial.print("inverted ");
  Serial.println("Switch on:" + status_str(relay1_settings.switch_status));
}