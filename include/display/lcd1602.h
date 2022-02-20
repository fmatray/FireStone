#ifndef LCD1602_H
#define LCD1602_H

#include "display/lcdxxxx.h"

class LCD1602 : public LCDXXXX {
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

  /* Start */
  void start();
  void show_mode();

  /* Menu */
  bool menu();
  void settings_saved();
  void settings_reset();
  void settings_reloaded();

 private:
  typedef void (LCD1602::*lcd1602_ptr)();
  typedef struct
  {
    unsigned char duration;
    lcd1602_ptr func;
    bool cond;
  } modes_s;

  /* helpers */
  void show_datetime();
  void show_ambiant();
  void show_relays();
  void show_octoprint_status();
  void show_printer_status();
  void show_temp(const String item, const int temp, const int target, const int offset, const status_e status);
  void show_ext0_temp();
  void show_ext1_temp();
  void show_bed_temp();
  void show_alert();
};

#endif