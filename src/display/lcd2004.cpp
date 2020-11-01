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

void LCD2004::run() {
  static unsigned long display_lasttime = 0;
  static unsigned long change_lasttime  = 0;
  static unsigned long blink_lasttime   = 0;
  static unsigned char mode_iter        = 0;
  static unsigned char brightness       = 255;

  if (Display::menu()) {
    lines[0] = lines[1] = lines[2] = lines[3] = "";
    return;
  }
  switch (action) {
    case nothing:
      break;
    case off:
    case sleep:
      lcd.setBacklight(0);
      clear();
      return;
    case wakeup:
    case restore:
      lcd.setBacklight(255);
      break;
    case err:
    case warn:
    case alert:
      if (check_time(blink_lasttime, 1)) {
        brightness = brightness == 255 ? 0 : 255;
        lcd.setBacklight(brightness);
        blink_lasttime = millis();
      }
      print2lines(action_str(action), message);
      return;
  }

  modes_s modes[] = {
      {5, &LCD2004::show_datetime_ambiant, true},
      {2, &LCD2004::show_octoprint_printer_status, true},
      {5, &LCD2004::show_printer_temps, octoprint.is_octoprint_connected()},
      {2, &LCD2004::show_relays, true},
      {0, NULL, false}};

  if (!check_time(display_lasttime, 1))
    return;
  display_lasttime = millis();

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
  print4lines(rtc_time() + "    " + rtc_date(), "",
              format_value("Temp", ambiant_sensor.get_temperature(), "C", ambiant_sensor.get_temperature_status()),
              format_value("Humid", ambiant_sensor.get_humidity(), "%", ambiant_sensor.get_humidity_status()));
}

/* Sensors */

void LCD2004::show_relays() {
  print4lines(
      "Relay 1:" + relay1.get_state_as_str(),
      "Relay 2:" + relay2.get_state_as_str(),
      "Relay 3:" + relay3.get_state_as_str(),
      "Relay 4:" + relay4.get_state_as_str());
}

void LCD2004::show_octoprint_printer_status() {
  printlinen("Octoprint" + octoprint.get_octoprint_status_as_str(), 1, true);

  if (octoprint.is_printer_operational()) {
    printlinen("Printer" + octoprint.get_printer_status_as_str(), 1, true);
    if (octoprint.is_working())
      printlinen(String(octoprint.get_completion()) + "%", 2, true);

  } else {
    printlinen("Printer", 1, true);
    printlinen("not operational", 2, true);
  }
}

void LCD2004::show_printer_temps() {
  print4lines(
      temp_to_string("Ex0",
                     octoprint.get_ext0_temp(), octoprint.get_ext0_target(), octoprint.get_ext0_offset(),
                     octoprint.get_ext0_status()),
      temp_to_string("Ex1",
                     octoprint.get_ext1_temp(), octoprint.get_ext1_target(), octoprint.get_ext1_offset(),
                     octoprint.get_ext1_status()),
      temp_to_string("Bed",
                     octoprint.get_bed_temp(), octoprint.get_bed_target(), octoprint.get_bed_offset(),
                     octoprint.get_bed_status()),
      "");
  printlinen(status_str(max(max(octoprint.get_ext0_status(), octoprint.get_ext1_status()), octoprint.get_bed_status())),
             3, true);
}

String LCD2004::temp_to_string(const String item,
                               const int temp, const int target, const int offset,
                               const status_e status) {
  return item + ":" + String(temp) + CELCIUS + " (" + String(target) + "+" + offset + ")";
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