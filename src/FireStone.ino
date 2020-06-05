#include <Arduino.h>
#include <Scheduler.h>
#include <WDTZero.h>

#include "common/debug.h"
#include "common/global.h"
#include "common/helpers.h"
#include "common/settings.h"
#include "config.h"
#include "controler/controler.h"

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

#ifdef FS_DEBUG
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

  /* IO SETUP */
  display.IO_setup();
  ambiant_sensor.begin();
  ambiant_sensor.update(settings.ambiant_settings);
  display.IO_test("Ambient Sensor", ambiant_sensor.test());

  emergency_sensor.begin();

  fire_sensor.begin();

  octoprint.begin();
  octoprint.update(settings.octoprint_settings);
  bool octoprint_test = octoprint.test();
  display.IO_test("Octoprint", octoprint_test, "Connected", "Unreachable");
  if (octoprint_test)
    display.octoprint_version();

  /* CONTROLLER SETUP */
  controler.begin();

  /* WatchDog SETUP */
  display.watchdog_setup();
  medor.attachShutdown(shutdown);
  medor.setup(WDT_SOFTCYCLE1M);

  settings.update();
  display.start();
  Scheduler.startLoop(loop2);
  Scheduler.startLoop(loop1);
#ifdef FS_DEBUG
  freememory();
#endif
}

#ifdef FS_DEBUG
unsigned long start       = 0;
unsigned long timer_loop0 = 0;
unsigned long timer_loop1 = 0;
unsigned long timer_loop2 = 0;
#endif

void loop() {
#ifdef FS_DEBUG
  static unsigned long freememory_lasttime = 0;

  if (check_time(freememory_lasttime, 10)) {
    freememory();
    freememory_lasttime = millis();
  }
  timer_loop0 = millis();
#endif

  medor.clear();  // Restart Watchdog

  if (settings.has_changed())
    settings.update();
  wificonnexion.run();
  octoprint.run_sensor();
  delay(100);
#ifdef FS_DEBUG
  if (millis() - timer_loop0 > 5000) {
    Serial.print("Loop0: ");
    Serial.println(millis() - timer_loop0);
  }
#endif
}

void loop1() {
#ifdef FS_DEBUG
  timer_loop1 = millis();
#endif
  ambiant_sensor.run();
  emergency_sensor.run();
  fire_sensor.run();

  /* Run Controler */
  controler.run();

  /* Run Actions */
  octoprint.run_action();
  relay1.run();
  relay2.run();
  relay3.run();
  relay4.run();
#ifdef FS_DEBUG
  if (millis() - timer_loop1 > 5000) {
    Serial.print("Loop1: ");
    Serial.println(millis() - timer_loop1);
  }
#endif
  yield();
}

void loop2() {
#ifdef FS_DEBUG
  timer_loop2 = millis();
#endif
  buzzer.run();
  display.run();
#ifdef FS_DEBUG
  if (millis() - timer_loop2 > 5000) {
    Serial.print("Loop2: ");
    Serial.println(millis() - timer_loop2);
  }
#endif
  yield();
}

/* Shutdown by watchdog */
void shutdown() {
  title("HO NO! Something went wrong...");
  title("You can start to pray.");
  title("We must reset!");
  display.shutdown();

#ifdef FS_DEBUG
  Serial.print("Now:");
  Serial.println(millis());
  Serial.print("Start:");
  Serial.println(millis() - start);
  Serial.print("Loop 0:");
  Serial.println(millis() - timer_loop0);
  Serial.print("Loop 1:");
  Serial.println(millis() - timer_loop1);
  Serial.print("Loop 2:");
  Serial.println(millis() - timer_loop2);

  freememory();
  while (1)
    ;
#endif
}