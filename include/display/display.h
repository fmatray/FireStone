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
  virtual void welcome();
  virtual void settings_setup();
  virtual void settings_setup_done();
  virtual void wifi_setup();
  virtual void wifi_setup_done();
  virtual void IO_setup();
  virtual void IO_test(String item, bool test, String ok = "OK", String ko = "KO");
  virtual void octoprint_version();
  virtual void watchdog_setup();

  /* Run */
  virtual void start();
  virtual void run();
  virtual void reset();

  /* Menu */
  bool menu();
  virtual void settings_saved();
  virtual void settings_reset();
  virtual void settings_reloaded();

  virtual void clear();
  /* Shutdown */
  virtual void shutdown();

 private:
};

#endif