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

String LCDXXXX::format_value(String item, int value, String unit, status_e status) {
  return item + ":" +
         String(value) + (unit == "C" ? CELCIUS : unit) +
         " " + status_str(status);  //char 223 for °
}

/* Helpers */
void LCDXXXX::printlinen(const String line, uint8_t n, bool center, short _delay, bool _clear) {
  lines[n] = "";
  for (char i = 0; i < center * (NB_COL - line.length()) / 2; i++)
    lines[n] += " ";
  lines[n] = lines[n]+ line + lines[n];
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