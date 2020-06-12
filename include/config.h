#ifndef CONFIG_H
#define CONFIG_H
#include <WiFiNINA.h>

#include "secrets.h"
/*
   Secrets
   Create a file with the following variables
*/
//const char WIFI_SSID[] = "";   // your network SSID (name)
//const char WIFI_PASSWORD[] = "";  // your network password
//const char OCTOPRINT_APIKEY[] = ""; // Octoprint Key
//const char MQTT_USER[] = "";
//const char MQTT_PASSWORD[] = "";

#define MDNS_NAME "firestone"

/* Debug */
#define FS_DEBUG

/* MQTT */
#define MQTT_HOST "192.168.1.110"  // Mosquitto server
#define MQTT_PORT 1883             // Mosquitto port

/* AMBIANT SENSOR */
#define TEMP_HUMIDITY_PIN 0  // DHT11 or DHT22 pin

/* EMERGENCY */
#define EMERGENCY_PIN       2  // RED EMERGENCY BUTTON
#define EMERGENCY_RESET_PIN 3  // reset button

/* FIRE */
#define FIRE_PIN 1  // Fire detector

/* OCTOPRINT */
const IPAddress OCTOPRINT_IP(192, 168, 1, 110);  // Octoprint server
#define OCTOPRINT_PORT 80                        // Octoprint port

/*
 * ACTIONS
 */
/* Buzzer */
#define BUZZER_PIN    A3
#define BUZZER_ACTIVE false

/* Relays */
#define RELAY1_PIN 4
#define RELAY2_PIN 5
#define RELAY3_PIN 6
#define RELAY4_PIN 7

/*
 * Display
 */

#define DISPLAY_TYPE LCD1602
#define LCD_ADDRESS  0x3F  // I2C LCD 1602 ADDRESS

/*
 * Input
 */
//#define ENCODER_INPUT
#define KEYS_INPUT  // ENCODER OR KEYS

#ifdef ENCODER_INPUT
#define ENCODER_A      8   //encoder A
#define ENCODER_B      9   //encoder B
#define ENCODER_BUTTON A4  //this encoder has a button here
#endif

#ifdef KEYS_INPUT  //negative pin numbers use internal pull-up
#define KEY_UP     -8
#define KEY_DOWN   -9
#define KEY_SELECT -A5
#define KEY_ESC    -A4  //Optional
#endif

#endif