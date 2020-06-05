#include <Arduino.h>
#include <RTCZero.h>

#include "common/wificonnexion.h"
#include "config.h"

/* SENSORS */
#include "IO/ambiant.h"
#include "IO/emergency.h"
#include "IO/fire.h"
#include "IO/octoprint.h"

/* ACTIONS */
#include "IO/buzzer.h"
#include "IO/octoprint.h"
#include "IO/relay.h"

/* Clock */
RTCZero rtc;

/* Communication */
WifiConnexion wificonnexion(WIFI_SSID, WIFI_PASSWORD);

/* IO */
AmbiantSensor ambiant_sensor(TEMP_HUMIDITY_PIN);
EmergencySensor emergency_sensor(EMERGENCY_PIN);
FireSensor fire_sensor(FIRE_PIN);
OctoPrint octoprint;
BuzzerAction buzzer(BUZZER_PIN, BUZZER_ACTIVE);
RelayAction relay1(RELAY1_PIN);
RelayAction relay2(RELAY2_PIN);
RelayAction relay3(RELAY3_PIN);
RelayAction relay4(RELAY4_PIN);