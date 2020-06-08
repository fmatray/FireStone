#ifndef LCD1602_H
#define LCD1602_H

#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>

#include "common/status.h"
#include "display.h"

class LCD1602 : public Display {
 public:
  void begin();
  /* Setup */
  void welcome();
  void settings_setup();
  void settings_setup_done();
  void wifi_setup();
  void wifi_update_fw();
  void wifi_setup_done();
  void IO_setup();
  void IO_test(String item, bool test, String ok = "OK", String ko = "KO");
  void octoprint_version();
  void watchdog_setup();

  /* Start */
  void start();
  void run();
  void reset();
  void clear();

  /* Menu */
  bool menu();
  void settings_saved();
  void settings_reset();
  void settings_reloaded();

  /* Shutdown */
  void shutdown();

 private:
  String l1, l2 = "";
  /* helpers */
  void print1line(const String line1, short _delay = 0, bool _clear = false);
  void print2lines(const String line1, const String line2, short _delay = 0, bool _clear = false);
  void show_datetime();
  String format_value(String item, int value, String unit, status_e status);
  void show_ambiant();
  void show_relays();
  void show_octoprint_status();
  void show_printer_status();
  void show_temp(const String item, const int temp, const int target, const int offset, const status_e status);
  void show_ext0_temp();
  void show_ext1_temp();
  void show_bed_temp();
};

typedef void (LCD1602::*lcd162_ptr)();
typedef struct
{
  unsigned char duration;
  lcd162_ptr func;
  bool cond;
} modes_s;

#endif