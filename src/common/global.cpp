#include <Arduino.h>
#include <RTCZero.h>

#include "common/mqtt.h"
#include "common/wificonnexion.h"
#include "config.h"

/* IO */
#include "IO/ambiant.h"
#include "IO/aq.h"
#include "IO/buzzer.h"
#include "IO/emergency.h"
#include "IO/fire.h"
#include "IO/octoprint.h"
#include "IO/relay.h"
#include "IO/timer.h"
/* Clock */
RTCZero rtc;

/* Communication */
WifiConnexion wificonnexion(WIFI_SSID, WIFI_PASSWORD);
MQTT mqtt;

/* IO */
AmbiantSensor ambiant_sensor;
AirQuality air_quality;
EmergencySensor emergency_sensor;
FireSensor fire_sensor;
TimerSensor timer;
OctoPrint octoprint;
BuzzerAction buzzer;
RelayAction relay1(RELAY1_PIN);
RelayAction relay2(RELAY2_PIN);
RelayAction relay3(RELAY3_PIN);
RelayAction relay4(RELAY4_PIN);

#if defined(DISPLAY_TYPE) && (DISPLAY_TYPE == 1602 || DISPLAY_TYPE == 2004)
#include <LiquidCrystal_PCF8574.h>
LiquidCrystal_PCF8574 lcd(LCD_ADDRESS);
#endif