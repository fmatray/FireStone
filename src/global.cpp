#include <Arduino.h>
#include <RTCZero.h>

#include "config.h"
#include "wificonnexion.h"

/* SENSORS */
#include "sensors/ambiant.h"
#include "sensors/emergency.h"
#include "sensors/fire.h"
#include "sensors/octoprint.h"

/* ACTIONS */
#include "actions/buzzer.h"
#include "actions/relay.h"

/* Clock */
RTCZero rtc;

/* Communication */
WifiConnexion wificonnexion(WIFI_SSID, WIFI_PASSWORD);

/* Sensors */
AmbiantSensor ambiant_sensor(TEMP_HUMIDITY_PIN);
EmergencySensor emergency_sensor(EMERGENCY_PIN);
FireSensor fire_sensor(FIRE_PIN);
OctoPrintSensor octoprint_sensor(OCTOPRINT_IP, OCTOPRINT_PORT, OCTOPRINT_APIKEY);

/*  Actions */
BuzzerAction buzzer(BUZZER_PIN, BUZZER_ACTIVE);
RelayAction relay1(RELAY1_PIN);
RelayAction relay2(RELAY2_PIN);
RelayAction relay3(RELAY3_PIN);
RelayAction relay4(RELAY4_PIN);