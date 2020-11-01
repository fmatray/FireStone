#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>
#include <IO/action.h>

#define MENU_MAX_DEPTH 3
#define MENU_TIMEOUT   10

#define RELAY_INVERTED(setting, menu) TOGGLE(setting, menu, "Inverted:", Menu::doNothing, Menu::noEvent, Menu::noStyle, \
                                             VALUE("Yes", true, update_settings, Menu::updateEvent),                    \
                                             VALUE("No", false, update_settings, Menu::updateEvent));

#define RELAY_SWITCH_STATUS(setting, menu) TOGGLE(setting, menu, "Switch:", Menu::doNothing, Menu::noEvent, Menu::noStyle, \
                                                  VALUE("Error", err, update_settings, Menu::updateEvent),                 \
                                                  VALUE("Warning", warn, update_settings, Menu::updateEvent),              \
                                                  VALUE("Alert", alert, update_settings, Menu::updateEvent));

#define TIMEOUT_MENU(setting, menu, title) TOGGLE(setting, menu, title, Menu::doNothing, Menu::noEvent, Menu::noStyle, \
                                                  VALUE("Off", 0, update_settings, Menu::updateEvent),                 \
                                                  VALUE("1", 1, update_settings, Menu::updateEvent),                   \
                                                  VALUE("5", 5, update_settings, Menu::updateEvent),                   \
                                                  VALUE("10", 10, update_settings, Menu::updateEvent),                 \
                                                  VALUE("15", 15, update_settings, Menu::updateEvent),                 \
                                                  VALUE("30", 30, update_settings, Menu::updateEvent),                 \
                                                  VALUE("60", 60, update_settings, Menu::updateEvent));

class Display : public Action {
 public:
  virtual void begin();
  /* Setup */
  virtual void welcome()                                                           = 0;
  virtual void settings_setup(bool success)                                        = 0;
  virtual void wifi_setup()                                                        = 0;
  virtual void wifi_update_fw()                                                    = 0;
  virtual void wifi_setup_done()                                                   = 0;
  virtual void IO_setup()                                                          = 0;
  virtual void IO_test(String item, bool test, String ok = "OK", String ko = "KO") = 0;
  virtual void octoprint_setup()                                                   = 0;
  virtual void octoprint_setup_done(bool success)                                  = 0;
  virtual void octoprint_version()                                                 = 0;
  virtual void watchdog_setup()                                                    = 0;
  virtual void mqtt_setup()                                                        = 0;
  virtual void mqtt_setup_done(bool success)                                       = 0;

  /* Run */
  virtual void start() = 0;
  virtual void run()   = 0;
  virtual void reset() = 0;

  /* Menu */
  bool menu();
  virtual void settings_saved()    = 0;
  virtual void settings_reset()    = 0;
  virtual void settings_reloaded() = 0;

  virtual void clear();
  /* Shutdown */
  virtual void shutdown();

 private:
};

#endif