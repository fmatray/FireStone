#ifndef LCD2004_H
#define LCD2004_H

#include "display/lcdxxxx.h"

class LCD2004 : public LCDXXXX {
 public:
  /* Setup */
  void welcome();
  void settings_setup(bool success);
  void wifi_setup();
  void wifi_update_fw();
  void wifi_setup_done();
  void IO_setup();
  void IO_test(String item, bool test, String ok = "OK", String ko = "KO");
  void octoprint_setup();
  void octoprint_setup_done(bool success);
  void octoprint_version();
  void watchdog_setup();
  void mqtt_setup();
  void mqtt_setup_done(bool success);

  /* Start */
  void start();

  /* Menu */
  bool menu();
  void settings_saved();
  void settings_reset();
  void settings_reloaded();

 private:
  typedef void (LCD2004::*lcd2004_ptr)();
  typedef struct
  {
    unsigned char duration;
    lcd2004_ptr func;
    bool cond;
  } modes_s;
  void show_mode();
  /* helpers */
  void print4lines(const String line1, const String line2, const String line3, const String line4,
                   short _delay = 0, bool _clear = false);
  void show_datetime_ambiant();
  void show_relays();
  void show_octoprint_printer_status();
  String temp_to_string(const String item, const int temp, const int target, const int offset);
  void show_printer_temps();
  void show_alert();
};

#endif