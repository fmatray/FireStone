#ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>
#include <RTCZero.h>
#include <WDTZero.h>

/* Communication */
#include "common/wificonnexion.h"

/* SENSORS */
#include "sensors/ambiant.h"
#include "sensors/emergency.h"
#include "sensors/fire.h"
#include "sensors/octoprint.h"

/* Actions */
#include "actions/buzzer.h"
#include "actions/octoprint.h"
#include "actions/relay.h"

/* Display */
#include "display/lcd1602.h"

/* Clock */
extern RTCZero rtc;

/* Communication */
extern WifiConnexion wificonnexion;

/* Sensors */
extern AmbiantSensor ambiant_sensor;
extern EmergencySensor emergency_sensor;
extern FireSensor fire_sensor;
extern OctoPrintSensor octoprint_sensor;

/*  Actions */
extern BuzzerAction buzzer;
extern OctoPrintAction octoprint_action;
extern RelayAction relay1;
extern RelayAction relay2;
extern RelayAction relay3;
extern RelayAction relay4;

/* WatchDog */
extern WDTZero medor;  // Define WatchDog

/* Display */
#if DISPLAY_TYPE == LCD1602
extern LCD1602 display;
extern LiquidCrystal_PCF8574 lcd;
#define CELCIUS "\xdf\x43"
#define NB_COL  16
#define NB_LINE 2
#else
#define CELCIUS "°C"
#error "NO DISPLAY DEFINED"
#endif
#endif