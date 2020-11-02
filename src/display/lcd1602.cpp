#include "common/global.h"
#include "common/helpers.h"
#include "config.h"
#include "display/lcd1602.h"

void LCD1602::welcome() { print2lines("Welcome", "to FireStone", 1, true); }

void LCD1602::settings_setup(bool success) {
  if (success) {
    print2lines("Configuration", "loaded", 1, true);
  } else {
    print2lines("Default conf.", "loaded", 1, true);
  }
};

void LCD1602::wifi_setup() { print2lines("WIFI Setup", wificonnexion.get_ssid()); }
void LCD1602::wifi_update_fw() { print2lines("Please Update", "the Wifi driver", 5); };
void LCD1602::wifi_setup_done() {
  if (wificonnexion.is_connected()) {
    print2lines("WIFI Setup OK", wificonnexion.get_ip_as_string(), 1, true);
    print2lines("WIFI Setup OK", "Sig:" + wificonnexion.quality(), 1, true);
  } else
    print2lines("WIFI Setup KO", wificonnexion.get_ssid());
}

void LCD1602::IO_setup() { print1line("IO Setup", 1, true); }
void LCD1602::IO_test(String item, bool test, String ok, String ko) { print2lines(item, test ? ok : ko, 1); }
void LCD1602::octoprint_setup() { print2lines("Connecting", "to Octoprint", 1, true); };
void LCD1602::octoprint_setup_done(bool success) {
  if (success) {
    print2lines("Connected", "to Octoprint", 1, true);
    octoprint_version();
  } else
    print2lines("Not connected", "to Octoprint", 1, true);
};
void LCD1602::octoprint_version() {
  print2lines("Octoprint V" + octoprint.get_octoprint_version(),
              "API V" + octoprint.get_api_version(), 1, true);
}

void LCD1602::watchdog_setup() { print2lines("Watchdog Setup", "60s", 1, true); }

void LCD1602::mqtt_setup() { print2lines("Connecting", "to MQTT", 1, true); };
void LCD1602::mqtt_setup_done(bool success) {
  if (success)
    print2lines("Connected", "to MQTT", 1, true);
  else
    print2lines("Not connected", "to MQTT", 1, true);
};

/* run */
void LCD1602::start() { print2lines("Starting", "", 1, true); }
void LCD1602::show_mode() {
  static unsigned long change_lasttime = 0;
  static unsigned char mode_iter       = 0;

  modes_s modes[] = {
      {2, &LCD1602::show_datetime, rtc.isConfigured()},
      {2, &LCD1602::show_octoprint_status, !octoprint.is_printer_operational()},
      {3, &LCD1602::show_printer_status, octoprint.is_octoprint_connected()},
      {5, &LCD1602::show_ext0_temp, octoprint.is_ext0_available()},
      {5, &LCD1602::show_ext1_temp, octoprint.is_ext1_available()},
      {5, &LCD1602::show_bed_temp, octoprint.is_bed_available()},
      {3, &LCD1602::show_ambiant, true},
      {2, &LCD1602::show_relays, true},
      {0, NULL, false}};

  (this->*modes[mode_iter].func)();
  if (check_time(change_lasttime, modes[mode_iter].duration)) {
    change_lasttime = millis();
    while (modes[++mode_iter].func != NULL)  // Look for next available mode
      if (modes[mode_iter].cond)
        return;
    mode_iter = 0;
  }
}

void LCD1602::show_datetime() { print2lines("    " + rtc_date(), "    " + rtc_time()); }

/* Sensors */
void LCD1602::show_ambiant() {
  print2lines(format_value("T", ambiant_sensor.get_temperature(), "C", ambiant_sensor.get_temperature_status()),
              format_value("H", ambiant_sensor.get_humidity(), "%", ambiant_sensor.get_humidity_status()));
}

void LCD1602::show_relays() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("R1:" + relay1.get_state_as_str());
  lcd.setCursor(8, 0);
  lcd.print("R2:" + relay2.get_state_as_str());
  lcd.setCursor(0, 1);
  lcd.print("R3:" + relay3.get_state_as_str());
  lcd.setCursor(8, 1);
  lcd.print("R4:" + relay4.get_state_as_str());
}

void LCD1602::show_octoprint_status() {
  print2lines("Octoprint", octoprint.get_octoprint_status_as_str());
}

void LCD1602::show_printer_status() {
  if (octoprint.is_printer_operational()) {
    String line2 = octoprint.get_printer_status_as_str();
    if (octoprint.is_working())
      line2 += ":" + String(octoprint.get_completion()) + "%";

    print2lines("Printer", line2);
  } else
    print2lines("Printer", "Not operational");
}

void LCD1602::show_ext0_temp() {
  show_temp("E0",
            octoprint.get_ext0_temp(), octoprint.get_ext0_target(), octoprint.get_ext0_offset(),
            octoprint.get_ext0_status());
}

void LCD1602::show_ext1_temp() {
  show_temp("E1",
            octoprint.get_ext1_temp(), octoprint.get_ext1_target(), octoprint.get_ext1_offset(),
            octoprint.get_ext1_status());
}

void LCD1602::show_bed_temp() {
  show_temp("Bed",
            octoprint.get_bed_temp(), octoprint.get_bed_target(), octoprint.get_bed_offset(),
            octoprint.get_bed_status());
}

void LCD1602::show_temp(const String item,
                        const int temp, const int target, const int offset,
                        const status_e status) {
  print2lines(item + ":" + String(temp) + CELCIUS + "/" + String(target) + "+" + offset,
              status_str(status));
}

void LCD1602::show_alert() {
  print2lines(action_str(action), message);
}

/* Menu */
void LCD1602::settings_saved() { print1line("Settings saved", 1); }
void LCD1602::settings_reset() { print1line("Settings reset", 1); }
void LCD1602::settings_reloaded() { print2lines("Settings", "reloaded", 1); }
