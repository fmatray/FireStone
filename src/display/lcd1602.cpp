#include "config.h"
#include "global.h"
#include "helpers.h"

LiquidCrystal_PCF8574 lcd(LCD_ADDRESS);

void LCD1602::begin() {
  lcd.begin(NB_COL, NB_LINE);
  lcd.clear();
  lcd.setBacklight(255);
  Display::begin();
}

void LCD1602::welcome() { print2lines("Welcome", "to FireStone", 1, true); }

void LCD1602::settings_setup() { print2lines("Loading", "configuration", 1, true); };
void LCD1602::settings_setup_done() { print2lines("Configuration", "loaded", 1, true); };

void LCD1602::wifi_setup() { print2lines("WIFI Setup", wificonnexion.get_ssid()); }
void LCD1602::wifi_setup_done() {
  if (wificonnexion.is_connected()) {
    print2lines("WIFI Setup OK", wificonnexion.get_ip_as_string(), 1, true);
    print2lines("WIFI Setup OK", "Sig:" + wificonnexion.quality(), 1, true);
  } else
    print2lines("WIFI Setup KO", wificonnexion.get_ssid());
}

void LCD1602::sensors_setup() { print1line("Sensors Setup", 1, true); }
void LCD1602::sensors_test(String item, bool test, String ok, String ko) { print2lines(item, test ? ok : ko, 1); }
void LCD1602::ocprint_version() {
  print2lines("Octoprint V" + octoprint_sensor.get_octoprint_version(),
              "API V" + octoprint_sensor.get_api_version(), 1, true);
}

void LCD1602::watchdog_setup() { print2lines("Watchdog Setup", "60s", 1, true); }

/* run */
void LCD1602::start() { print2lines("Starting", "", 1, true); }
void LCD1602::run() {
  static unsigned long display_lasttime = 0;
  static unsigned long change_lasttime  = 0;
  static unsigned long blink_lasttime   = 0;
  static unsigned char mode_iter        = 0;
  static unsigned char brightness       = 255;

  if (Display::menu())
    return;
  modes_s modes[] = {
      {2, &LCD1602::show_datetime, rtc.isConfigured()},
      {2, &LCD1602::show_octoprint_status, !octoprint_sensor.is_printer_operational()},
      {3, &LCD1602::show_printer_status, octoprint_sensor.is_octoprint_connected()},
      {5, &LCD1602::show_ext0_temp, octoprint_sensor.is_printer_operational()},
      {5, &LCD1602::show_ext1_temp, octoprint_sensor.is_printer_operational()},
      {5, &LCD1602::show_bed_temp, octoprint_sensor.is_printer_operational()},
      {3, &LCD1602::show_ambiant, true},
      {2, &LCD1602::show_relays, true},
      {0, NULL}};

  if (status == ERROR || status == WARNING || status == ALERT) {
    if (check_time(blink_lasttime, 1)) {
      brightness = brightness == 255 ? 0 : 255;
      lcd.setBacklight(brightness);
      blink_lasttime = millis();
    }
    print2lines(status_str(status), message);
    return;
  }
  lcd.setBacklight(255);

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

void LCD1602::clear() { lcd.clear(); }

void LCD1602::show_datetime() { print2lines("    " + rtc_date(), "    " + rtc_time()); }

/* Sensors */
String LCD1602::format_value(String item, int value, String unit, status_e status) {
  return item + ":" +
         String(value) + (unit == "C" ? CELCIUS : unit) +
         " " + status_str(status);  //char 223 for °
}

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
  print2lines("Octoprint", octoprint_sensor.get_octoprint_status_as_str());
}

void LCD1602::show_printer_status() {
  if (octoprint_sensor.is_printer_operational()) {
    String line2 = octoprint_sensor.get_printer_status_as_str();
    if (octoprint_sensor.is_working())
      line2 += ":" + String(octoprint_sensor.get_completion()) + "%";

    print2lines("Printer", line2);
  } else
    print2lines("Printer", "Not operational");
}

void LCD1602::show_ext0_temp() {
  show_temp("E0",
            octoprint_sensor.get_ext0_temp(), octoprint_sensor.get_ext0_target(), octoprint_sensor.get_ext0_offset(),
            octoprint_sensor.get_ext0_status());
}

void LCD1602::show_ext1_temp() {
  show_temp("E1",
            octoprint_sensor.get_ext1_temp(), octoprint_sensor.get_ext1_target(), octoprint_sensor.get_ext1_offset(),
            octoprint_sensor.get_ext1_status());
}

void LCD1602::show_bed_temp() {
  show_temp("Bed",
            octoprint_sensor.get_bed_temp(), octoprint_sensor.get_bed_target(), octoprint_sensor.get_bed_offset(),
            octoprint_sensor.get_bed_status());
}

void LCD1602::show_temp(const String item,
                        const int temp, const int target, const int offset,
                        const status_e status) {
  print2lines(item + ":" + String(temp) + CELCIUS + "/" + String(target) + "+" + offset,
              status_str(status));
}

/* Menu */
void LCD1602::settings_saved() { print1line("Settings saved", 1); }
void LCD1602::settings_reset() { print1line("Settings reset", 1); }
void LCD1602::settings_reloaded() { print2lines("Settings", "reloaded", 1); }

/* Helpers */
void LCD1602::print1line(const String line1, short _delay, bool _clear) { print2lines(line1, "", _delay, _clear); }

void LCD1602::print2lines(const String line1, const String line2, short _delay, bool _clear) {
  static String l1, l2 = "";

  if (l1 != line1 || l2 != line2) {
    l1 = line1;
    l2 = line2;
    clear();
    lcd.print(line1.c_str());
    lcd.setCursor(0, 1);
    lcd.print(line2.c_str());
  }
  if (_delay)
    delay(_delay * 1000);
  if (_clear)
    clear();
}

/* Shutdown */
void LCD1602::shutdown() { print2lines("Start to pray.", "We must reset!"); }