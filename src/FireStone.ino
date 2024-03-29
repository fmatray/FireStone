#include <Arduino.h>
#include <Scheduler.h>
#include <WDTZero.h>

#include "config.h"
#include "common/debug.h"
#include "common/global.h"
#include "common/helpers.h"
#include "common/settings.h"
#include "controler/controler.h"
/* Display */
#if DISPLAY_TYPE == 1602
LCD1602 display;
#elif DISPLAY_TYPE == 2004
LCD2004 display;
#else
#error "NO DISPLAY DEFINED"
#endif

WDTZero medor;  // Define WatchDog (medor is common name for a french dog)
Settings settings;
Controler controler;

void setup() {
  display.begin();
  display.clear();

  /* SERIAL FOR DEBUG */
#ifdef FS_DEBUG
  unsigned long wait_serial = millis();
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  while (!Serial && millis() - wait_serial < 10000)
    blink(LED_BUILTIN);
#endif
  /* WELCOME */
  display.welcome();

  /* LOAD SETTINGS */
  display.settings_setup(settings.begin());
  settings.print();

  /* RTC + WIFI Setup */
  rtc.begin();  // Clock setup
  display.wifi_setup();
  wificonnexion.update(settings.timezone_offset);
  wificonnexion.begin();
  display.wifi_setup_done();

  /* IO SETUP */
  display.IO_setup();

  air_quality.begin();
  air_quality.update(settings.air_quality_settings);
  display.IO_test("Air Quality Sensor", air_quality.test());


  ambiant_sensor.begin();
  ambiant_sensor.update(settings.ambiant_settings);
  display.IO_test("Ambient Sensor", ambiant_sensor.test());

  emergency_sensor.begin();
  fire_sensor.begin();

  /* Octoprint SETUP */
  display.octoprint_setup();
  octoprint.begin();
  octoprint.update(settings.octoprint_settings);
  display.octoprint_setup_done(octoprint.test());

  /* CONTROLLER SETUP */
  controler.begin();

  /* WatchDog SETUP */
  display.watchdog_setup();
  medor.attachShutdown(shutdown);
  medor.setup(WDT_SOFTCYCLE1M);

  /* Start loops */
  timer.clear();
  settings.update();
  display.start();
  Scheduler.startLoop(loop1);
  Scheduler.startLoop(loop2);
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

/* 
 *
 * MAIN LOOP
 * Watchdog and wifi connexion
 * 
 */

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

  wificonnexion.run();
  if (wificonnexion.is_connected())
    octoprint.run_sensor();
  delay(20);
#ifdef FS_DEBUG
  if (millis() - timer_loop0 > 5000) {
    Serial.print("Loop0: ");
    Serial.println(millis() - timer_loop0);
  }
#endif
}

/* 
 *
 * FIRST LOOP
 * sensors and actions
 * 
 */

void loop1() {
#ifdef FS_DEBUG
  timer_loop1 = millis();
#endif
  ambiant_sensor.run();
  air_quality.run();
  emergency_sensor.run();
  fire_sensor.run();
  timer.run();

  /* Run Controler */
  controler.run();

  /* Run Actions  !!!!!! DISABLED FOR NOW
  octoprint.run_action();

  relay1.run();
  relay2.run();
  relay3.run();
  relay4.run();
  */
 
#ifdef FS_DEBUG
  if (millis() - timer_loop1 > 5000) {
    Serial.print("Loop1: ");
    Serial.println(millis() - timer_loop1);
  }
#endif
  yield();
}

/* 
 *
 * SECOND LOOP
 * sensors and Buzzer
 * 
 */

void loop2() {
#ifdef FS_DEBUG
  timer_loop2 = millis();
#endif
  if (settings.has_changed())
    settings.update();
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

/* 
 * 
 * Shutdown by watchdog 
 * 
 */

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