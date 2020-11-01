#ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>
#include <RTCZero.h>
#include <WDTZero.h>

/* Communication */
#include "common/mqtt.h"
#include "common/wificonnexion.h"

/* IO */
#include "IO/ambiant.h"
#include "IO/buzzer.h"
#include "IO/emergency.h"
#include "IO/fire.h"
#include "IO/octoprint.h"
#include "IO/relay.h"
#include "IO/timer.h"

/* Display */

/* Clock */
extern RTCZero rtc;

/* Communication */
extern WifiConnexion wificonnexion;
extern MQTT mqtt;

/* IO */
extern AmbiantSensor ambiant_sensor;
extern EmergencySensor emergency_sensor;
extern FireSensor fire_sensor;
extern TimerSensor timer;
extern OctoPrint octoprint;
extern BuzzerAction buzzer;
extern RelayAction relay1;
extern RelayAction relay2;
extern RelayAction relay3;
extern RelayAction relay4;

/* WatchDog */
extern WDTZero medor;  // Define WatchDog

/* Display */
#if defined(DISPLAY_TYPE) && DISPLAY_TYPE == 1602
#define NB_COL  16
#define NB_LINE 2
#include "display/lcd1602.h"
extern LCD1602 display;
extern LiquidCrystal_PCF8574 lcd;
#define CELCIUS "\xdf\x43"

#elif defined(DISPLAY_TYPE) && DISPLAY_TYPE == 2004
#define NB_COL  20
#define NB_LINE 4
#include "display/lcd2004.h"
extern LCD2004 display;
extern LiquidCrystal_PCF8574 lcd;
#define CELCIUS "\xdf\x43"
#else
#define CELCIUS "°C"
#error "NO DISPLAY DEFINED"
#endif


#endif