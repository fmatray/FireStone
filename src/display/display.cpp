#include "display/display.h"

#include <Arduino.h>

#include "common/global.h"
#include "common/settings.h"
#include "config.h"

#if DISPLAY_TYPE == LCD1602
#include <LiquidCrystal_PCF8574.h>
#include <menuIO/PCF8574Out.h>  //arduino I2C LCD
#endif
#include <menu.h>
#include <menuIO/chainStream.h>
#include <menuIO/keyIn.h>

// INPUTS

#ifdef ENCODER_INPUT
#include <menuIO/encoderIn.h>

encoderIn<ENCODER_A, ENCODER_B> encoder;                           // simple quad encoder driver
encoderInStream<ENCODER_A, ENCODER_B> encoder_stream(encoder, 4);  // simple quad encoder fake Stream
//a keyboard with only one key as the encoder button
keyMap encoder_button_map[] = {{-ENCODER_BUTTON, defaultNavCodes[enterCmd].ch}};  //negative pin numbers use internal pull-up, this is on when low
keyIn<1> encoder_button(encoder_button_map);                                      //1 is the number of keys

MENU_INPUTS(in, &encoder_stream, &encoder_button);
#endif

#ifdef KEYS_INPUT
keyMap key_map[] = {
    {KEY_SELECT, defaultNavCodes[enterCmd].ch},
    {KEY_UP, defaultNavCodes[upCmd].ch},
    {KEY_DOWN, defaultNavCodes[downCmd].ch},
    {KEY_ESC, defaultNavCodes[escCmd].ch},
};
keyIn<4> key_buttons(key_map);
MENU_INPUTS(in, &key_buttons);
#endif

// OUTPUTS
#if DISPLAY_TYPE == LCD1602
MENU_OUTPUTS(out, MENU_MAX_DEPTH, LCD_OUT(lcd, {0, 0, NB_COL, NB_LINE}), NONE);
#else
MENU_OUTPUTS(out, MENU_MAX_DEPTH, SERIAL_OUT(Serial), NONE);
#endif

/* Settings menu */
extern Settings settings;
result update_settings(eventMask event, navNode &nav, prompt &item) {
  settings.changed();
  return proceed;
}

result save_settings() {
  settings.save();
  display.settings_saved();
  out.refresh();
  return proceed;
}

result reset_settings() {
  settings.reset();
  settings.changed();
  display.settings_reset();
  out.refresh();
  return proceed;
}

result reload_settings() {
  settings.reset();
  settings.changed();
  display.settings_reloaded();
  out.refresh();
  return proceed;
}

/* MENU */
MENU(ambiant_menu, "Ambiant", Menu::doNothing, Menu::noEvent, Menu::noStyle,
     FIELD(settings.ambiant_settings.temp_ambient_offset, "T Offset", CELCIUS, -5, 5, 1, 0.1, update_settings, Menu::updateEvent, Menu::noStyle),
     FIELD(settings.ambiant_settings.humid_ambient_offset, "H Offset", CELCIUS, -5, 5, 1, 0.1, update_settings, Menu::updateEvent, Menu::noStyle),
     FIELD(settings.ambiant_settings.min_temp_ambiant, "T Min", CELCIUS, 0, 50, 5, 1, update_settings, Menu::updateEvent, Menu::noStyle),
     FIELD(settings.ambiant_settings.max_temp_ambiant, "T Max", CELCIUS, 0, 50, 5, 1, update_settings, Menu::updateEvent, Menu::noStyle),
     FIELD(settings.ambiant_settings.max_humid_ambiant, "H Max", "%", 0, 100, 5, 1, update_settings, Menu::updateEvent, Menu::noStyle),
     EXIT("<Back"));

MENU(octoprint_menu, "Octoprint", Menu::doNothing, Menu::noEvent, Menu::noStyle,
     FIELD(settings.octoprint_settings.interval, "Interval", "s", 5, 30, 5, 1, update_settings, Menu::updateEvent, Menu::noStyle),
     FIELD(settings.octoprint_settings.max_temp_ext0, "E0 Max", CELCIUS, 0, 500, 10, 5, update_settings, Menu::updateEvent, Menu::noStyle),
     FIELD(settings.octoprint_settings.max_temp_ext1, "E1 Max", CELCIUS, 0, 500, 10, 5, update_settings, Menu::updateEvent, Menu::noStyle),
     FIELD(settings.octoprint_settings.max_temp_bed, "Bed Max", CELCIUS, 0, 120, 10, 5, update_settings, Menu::updateEvent, Menu::noStyle),
     EXIT("<Back"));

RELAY_INVERTED(settings.relay1_settings.inverted, relay1_inverted_menu);
RELAY_SWITCH_STATUS(settings.relay1_settings.switch_status, relay1_switch_status_menu);
MENU(relay1_menu, "Relay 1", Menu::doNothing, Menu::noEvent, Menu::noStyle,
     SUBMENU(relay1_inverted_menu), SUBMENU(relay1_switch_status_menu),
     EXIT("<Back"));
RELAY_INVERTED(settings.relay2_settings.inverted, relay2_inverted_menu);
RELAY_SWITCH_STATUS(settings.relay2_settings.switch_status, relay2_switch_status_menu);
MENU(relay2_menu, "Relay 2", Menu::doNothing, Menu::noEvent, Menu::noStyle,
     SUBMENU(relay2_inverted_menu), SUBMENU(relay2_switch_status_menu),
     EXIT("<Back"));
RELAY_INVERTED(settings.relay3_settings.inverted, relay3_inverted_menu);
RELAY_SWITCH_STATUS(settings.relay3_settings.switch_status, relay3_switch_status_menu);
MENU(relay3_menu, "Relay 3", Menu::doNothing, Menu::noEvent, Menu::noStyle,
     SUBMENU(relay3_inverted_menu), SUBMENU(relay3_switch_status_menu),
     EXIT("<Back"));
RELAY_INVERTED(settings.relay4_settings.inverted, relay4_inverted_menu);
RELAY_SWITCH_STATUS(settings.relay4_settings.switch_status, relay4_switch_status_menu);
MENU(relay4_menu, "Relay 4", Menu::doNothing, Menu::noEvent, Menu::noStyle,
     SUBMENU(relay4_inverted_menu), SUBMENU(relay4_switch_status_menu),
     EXIT("<Back"));

MENU(relays_menu, "Relays", Menu::doNothing, Menu::noEvent, Menu::noStyle,
     SUBMENU(relay1_menu),
     SUBMENU(relay2_menu),
     SUBMENU(relay3_menu),
     SUBMENU(relay4_menu),
     EXIT("<Back"));

TIMEOUT_MENU(settings.timer_settings.idle_timeout, idle_timeout_menu, "Idle:");
TIMEOUT_MENU(settings.timer_settings.off_timeout, off_timeout_menu, "Off:");

MENU(advanced_menu, "Advanced", Menu::doNothing, Menu::noEvent, Menu::noStyle,
     FIELD(settings.timezone_offset, "Timezone", "h", -12, 12, 1, 0, update_settings, Menu::updateEvent, Menu::noStyle),
     SUBMENU(idle_timeout_menu), SUBMENU(off_timeout_menu),
     OP("Reset settings", reset_settings, enterEvent),
     OP("Reload settings", reload_settings, enterEvent),
     EXIT("<Back"));

MENU(main_menu, "Menu", Menu::doNothing, Menu::noEvent, Menu::noStyle,
     SUBMENU(ambiant_menu),
     SUBMENU(octoprint_menu),
     SUBMENU(relays_menu),
     SUBMENU(advanced_menu),
     OP("Save settings", save_settings, enterEvent),
     EXIT("<Exit"));

// NAVROOT
NAVROOT(nav, main_menu, MENU_MAX_DEPTH, in, out);

void Display::begin() {
#ifdef ENCODER_INPUT
  encoder_button.begin();
  encoder.begin();
#endif
#ifdef KEYS_INPUT
  key_buttons.begin();
#endif
  nav.timeOut        = MENU_TIMEOUT;
  nav.useUpdateEvent = true;  // use UpdateEvent
  nav.idleOn();               // start without menu
}

bool Display::menu() {
  nav.poll();
  if (!nav.sleepTask)
    timer.clear();
  return !nav.sleepTask;
}