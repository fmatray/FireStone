#include "config.h"
#include "common/global.h"
#include "common/helpers.h"

#include "display/lcdxxxx.h"

void LCDXXXX::begin() {
  lcd.begin(NB_COL, NB_LINE);
  clear();
  lcd.setBacklight(255);
  Display::begin();
}

void LCDXXXX::reset() {
  lcd.setBacklight(255);
  clear();
}

void LCDXXXX::clear() { lcd.clear(); }

void LCDXXXX::run() {
  static unsigned long display_lasttime = 0;
  static unsigned long blink_lasttime   = 0;
  static uint8_t brightness             = 255;

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
      show_alert();
      return;
  }

  if (!check_time(display_lasttime, 1))
    return;
  display_lasttime = millis();

  show_mode();
}

String LCDXXXX::format_value(String item, int value, String unit, status_e status) {
  return item + ":" +
         String(value) + (unit == "C" ? CELCIUS : unit) +
         " " + status_str(status);  //char 223 for °
}

/* Helpers */
void LCDXXXX::clearline(uint8_t n) {
  lines[n] = "                    ";
  lcd.setCursor(0, n);
  lcd.print(lines[n]);
}

void LCDXXXX::printlinen(const String line, uint8_t n, bool center, short _delay, bool _clear) {
  if (lines[n] == line)
    return;
  clearline(n);
  lines[n] = line;
  if (center && lines[n].length() < NB_COL)
    lcd.setCursor((NB_COL - lines[n].length()) / 2, n);
  else
    lcd.setCursor(0, n);
  lcd.print(lines[n]);
  if (_delay)
    delay(_delay * 1000);
  if (_clear)
    clear();
}
void LCDXXXX::print1line(const String line1, short _delay, bool _clear) { print2lines(line1, "", _delay, _clear); }

void LCDXXXX::print2lines(const String line1, const String line2, short _delay, bool _clear) {
  if (lines[0] != line1 || lines[1] != line2) {
    lines[0] = line1;
    lines[1] = line2;
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
void LCDXXXX::shutdown() { print2lines("Start to pray.", "We must reset!"); }