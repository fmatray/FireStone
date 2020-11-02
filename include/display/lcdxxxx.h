#ifndef LCDXXXX_H
#define LCDXXXX_H

#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>

#include "common/status.h"
#include "display.h"

class LCDXXXX : public Display {
 public:
  void begin();
  void reset();
  void clear();
  /* Shutdown */
  void shutdown();
  void run();

 protected:
  virtual void show_mode() = 0;
  String lines[NB_LINE];
  void printlinen(const String line, uint8_t n, bool center = false, short _delay = 0, bool _clear = false);
  void print1line(const String line, short _delay = 0, bool _clear = false);
  void print2lines(const String line1, const String line2, short _delay = 0, bool _clear = false);
  void clearline(uint8_t n);

  String format_value(String item, int value, String unit, status_e status);
  String tmp_to_string(const String item, const int temp, const int target, const int offset, const status_e status);
};

#endif