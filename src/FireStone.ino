#include <Arduino.h>
#include <Scheduler.h>
#include <WDTZero.h>

#include "config.h"
#include "controler/controler.h"
#include "debug.h"
#include "global.h"
#include "helpers.h"
#include "settings.h"

/* Display */
#if DISPLAY_TYPE == LCD1602
#include "display/lcd1602.h"
LCD1602 display;
#else
#error "NO DISPLAY DEFINED"
#endif

WDTZero medor;  // Define WatchDog (medor is common name for a french dog)
Settings settings;
Controler controler(EMERGENCY_RESET_PIN);

void setup() {
  display.begin();
  display.clear();

#ifdef DEBUG
  unsigned long wait_serial = millis();
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  while (!Serial && millis() - wait_serial < 10000)
    blink(LED_BUILTIN);
#endif
  /* COMMUNICATION SETUP */
  display.welcome();

  display.settings_setup();
  settings.begin();
  settings.print();
  display.settings_setup_done();

  rtc.begin();  // Clock setup

  display.wifi_setup();
  wificonnexion.update(settings.timezone_offset);
  wificonnexion.begin();
  display.wifi_setup_done();

  /* SENSORS SETUP */
  display.sensors_setup();
  ambiant_sensor.begin();
  ambiant_sensor.update(settings.ambiant_settings);
  display.sensors_test("Ambient Sensor", ambiant_sensor.test());

  emergency_sensor.begin();

  fire_sensor.begin();

  octoprint_sensor.begin();
  octoprint_sensor.update(settings.octoprint_settings);
  bool octoprint_test = octoprint_sensor.test();
  display.sensors_test("Octoprint", octoprint_test, "Connected", "Unreachable");
  if (octoprint_test)
    display.ocprint_version();
  /* ACTIONS SETUP */

  /* CONTROLLER SETUP */
  controler.begin();

  /* WatchDog SETUP */
  display.watchdog_setup();
  medor.attachShutdown(shutdown);
  medor.setup(WDT_SOFTCYCLE1M);

  settings.update();
  display.start();
  Scheduler.startLoop(display_loop);
  Scheduler.startLoop(controler_loop);
#ifdef DEBUG
  freememory();
#endif
}

#ifdef DEBUG
unsigned long start = 0;
unsigned long loop0 = 0;
unsigned long loop1 = 0;
unsigned long loop2 = 0;
#endif

void loop() {
#ifdef DEBUG
  static unsigned long freememory_lasttime = 0;
  unsigned long start_time                 = millis();

  if (check_time(freememory_lasttime, 10)) {
    freememory();
    freememory_lasttime = millis();
  }
  loop0 = millis();
#endif

  medor.clear();  // Restart Watchdog

  if (settings.has_changed())
    settings.update();
  wificonnexion.run();
  octoprint_sensor.run();
  delay(100);
#ifdef DEBUG
  if (millis() - start_time > 5000) {
    Serial.print("Loop: ");
    Serial.println(millis() - start_time);
  }
#endif
}

void controler_loop() {
#ifdef DEBUG
  loop1 = millis();
#endif
  ambiant_sensor.run();
  emergency_sensor.run();
  fire_sensor.run();
  /* Run Controler */
  controler.run();

  /* Run Actions */
  buzzer.run();
  relay1.run();
  relay2.run();
  relay3.run();
  relay4.run();
  yield();
}

void display_loop() {
#ifdef DEBUG
  loop2 = millis();
#endif
  display.run();
  yield();
}

/* Shutdown by watchdog */
void shutdown() {
  title("HO NO! Something went wrong...");
  title("You can start to pray.");
  title("We must reset!");
  display.shutdown();

#ifdef DEBUG
  Serial.print("Start:");
  Serial.println(millis() - start);
  Serial.print("Loop 0:");
  Serial.println(millis() - loop0);
  Serial.print("Loop 1:");
  Serial.println(millis() - loop1);
  Serial.print("Loop 2:");
  Serial.println(millis() - loop2);

  freememory();
  while (1)
    ;
#endif
}