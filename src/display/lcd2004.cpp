#include "common/global.h"
#include "common/helpers.h"
#include "config.h"

#include "display/lcd2004.h"

void LCD2004::welcome() {
  printlinen("Welcome", 1, true);
  printlinen("to FireStone", 2, true, 2, true);
}

void LCD2004::settings_setup(bool success) {
  clear();
  if (success) {
    printlinen("Configuration", 1, true);
    printlinen("Loaded", 2, true, 2, true);
  } else {
    printlinen("Default", 1, true);
    printlinen("configuration loaded", 2, true);
  }
};

void LCD2004::wifi_setup() {
  clear();
  printlinen("Connecting WIFI", 0, true);
  printlinen(wificonnexion.get_ssid(), 1, true);
}
void LCD2004::wifi_update_fw() { print2lines("Please Update", "the Wifi driver", 5); };
void LCD2004::wifi_setup_done() {
  if (wificonnexion.is_connected()) {
    printlinen("WIFI:" + wificonnexion.quality(), 0, true);
    printlinen("@IP:" + wificonnexion.get_ip_as_string(), 3, true, 5, true);
  } else
    printlinen("WIFI Setup KO", 0, true, 2, true);
}

void LCD2004::IO_setup() { printlinen("IO Setup", 0, true); }
void LCD2004::IO_test(String item, bool test, String ok, String ko) {
  printlinen(item, 1, true);
  printlinen(test ? ok : ko, 2, true);
  delay(1000);
}

void LCD2004::octoprint_setup() {
  clear();
  printlinen("Octoprint", 0, true);
};

void LCD2004::octoprint_setup_done(bool success) {
  if (success) {
    printlinen("Connected", 1, true);
    printlinen("Version " + octoprint.get_octoprint_version(), 2, true);
    printlinen("API V" + octoprint.get_api_version(), 3, true);
  } else
    printlinen("Unreachable", 1, true);
  delay(2000);
  clear();
};
void LCD2004::octoprint_version() {
  print2lines("Octoprint V" + octoprint.get_octoprint_version(),
              "API V" + octoprint.get_api_version(), 1, true);
}

void LCD2004::watchdog_setup() {
  clear();
  printlinen("Watchdog Setup", 1, true);
  printlinen("60s", 2, true);
  delay(1000);
}

void LCD2004::mqtt_setup() {
  clear();
  printlinen("MQTT ", 1, true);
};

void LCD2004::mqtt_setup_done(bool success) {
  if (success)
    printlinen("Connected", 2, true);
  else
    printlinen("Connexion error", 2, true);
  delay(2000);
  clear();
};

/* run */
void LCD2004::start() {
  clear();
  printlinen("Starting", 1, true);
}

void LCD2004::show_mode() {
  static unsigned long change_lasttime = 0;
  static uint8_t mode_iter             = 0;

  modes_s modes[] = {
      {5, &LCD2004::show_datetime_ambiant, true},
      {5, &LCD2004::show_octoprint_printer_status, true},
      {5, &LCD2004::show_printer_temps, octoprint.is_printer_operational()},
      {2, &LCD2004::show_relays, true},
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

void LCD2004::show_datetime_ambiant() {
  printlinen(rtc_time() + "    " + rtc_date(), 0);
  clearline(1);
  printlinen(format_value("Temp", ambiant_sensor.get_temperature(), "C", ambiant_sensor.get_temperature_status()), 2);
  printlinen(format_value("Humid", ambiant_sensor.get_humidity(), "%", ambiant_sensor.get_humidity_status()), 3);
}

/* Sensors */

void LCD2004::show_relays() {
  printlinen("Relay 1:" + relay1.get_state_as_str(), 0);
  printlinen("Relay 2:" + relay2.get_state_as_str(), 1);
  printlinen("Relay 3:" + relay3.get_state_as_str(), 2);
  printlinen("Relay 4:" + relay4.get_state_as_str(), 3);
}

void LCD2004::show_octoprint_printer_status() {
  printlinen("Octoprint ", 0, true);
  printlinen(octoprint.get_octoprint_status_as_str(), 1, true);

  if (!octoprint.is_octoprint_connected()) {
    clearline(2);
    clearline(3);
    return;
  }
  if (octoprint.is_printer_operational()) {
    printlinen("Printer " + octoprint.get_printer_status_as_str(), 2, true);
    if (octoprint.is_working())
      printlinen(String(octoprint.get_completion()) + "%", 3, true);
    else
      clearline(3);
  } else {
    printlinen("Printer", 2, true);
    printlinen("not operational", 3, true);
  }
}

void LCD2004::show_printer_temps() {
  printlinen("Ext&Bed:" + status_str(max(max(octoprint.get_ext0_status(), octoprint.get_ext1_status()), octoprint.get_bed_status())), 0, true);
  if (octoprint.is_ext0_available())
    printlinen(temp_to_string("Ex0", octoprint.get_ext0_temp(), octoprint.get_ext0_target(), octoprint.get_ext0_offset()), 1);
  else
    clearline(1);
  if (octoprint.is_ext1_available())
    printlinen(temp_to_string("Ex1", octoprint.get_ext1_temp(), octoprint.get_ext1_target(), octoprint.get_ext1_offset()), 2);
  else
    clearline(2);
  if (octoprint.is_bed_available())
    printlinen(temp_to_string("Bed", octoprint.get_bed_temp(), octoprint.get_bed_target(), octoprint.get_bed_offset()), 3);
  else
    clearline(3);
}

String LCD2004::temp_to_string(const String item, const int temp, const int target, const int offset) {
  return item + ":" + String(temp) + CELCIUS + " (" + String(target) + "+" + offset + ")";
}

void LCD2004::show_alert() {
  clearline(0);
  printlinen(action_str(action), 1, true);
  printlinen(message, 2, true);
  clearline(3);
}

/* Menu */
void LCD2004::settings_saved() { print1line("Settings saved", 1); }
void LCD2004::settings_reset() { print1line("Settings reset", 1); }
void LCD2004::settings_reloaded() { print2lines("Settings", "reloaded", 1); }

/* Helpers */

void LCD2004::print4lines(const String line1, const String line2, const String line3, const String line4,
                          short _delay, bool _clear) {
  if (lines[0] != line1 || lines[1] != line2 || lines[2] != line3 || lines[3] != line4) {
    lines[0] = line1;
    lines[1] = line2;
    lines[2] = line3;
    lines[3] = line4;
    clear();
    lcd.print(line1.c_str());
    lcd.setCursor(0, 1);
    lcd.print(line2.c_str());
    lcd.setCursor(0, 2);
    lcd.print(line3.c_str());
    lcd.setCursor(0, 3);
    lcd.print(line4.c_str());
  }
  if (_delay)
    delay(_delay * 1000);
  if (_clear)
    clear();
}