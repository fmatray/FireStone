#include "common/wificonnexion.h"

#include <RTCZero.h>
#include <WiFiUdp.h>

#include "common/global.h"
#include "common/helpers.h"

/*
    SETUP
*/
WifiConnexion::WifiConnexion(const char *_ssid, const char *_password) {
  ssid     = _ssid;
  password = _password;
}
void WifiConnexion::begin() {
  title("Wifi Setup");

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true)
      ;
  }

  // Check Firmware
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
    Serial.print("Current version:");
    Serial.println(fv);
    display.wifi_update_fw();
  }
  connect_delay();
  if (!mdns_responder.begin(MDNS_NAME))
    Serial.println("Failed to start MDNS responder!");
}

/*
   RUN
*/

void WifiConnexion::run() {
  if (WiFi.status() == WL_CONNECTED) {
    updateRTC();
    mdns_responder.poll();
  } else
    connect();
}

void WifiConnexion::connect()  // TODO: ADD WPAEnterprise
{
  static unsigned long wifi_lasttime = 0;  //last time wifi connexion try has been done
  static char connexion_try          = 0;
  unsigned char wifi_delay;

  wifi_delay = connexion_try < WIFI_TRIES ? 10 : 180;

  if (!check_time(wifi_lasttime, wifi_delay))  // Try to reconnect after 10s or 3min if nb tries is reached
    return;
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.print(ssid);
  Serial.print(" (try ");
  Serial.print(connexion_try + 1);
  Serial.println(")");
  // Connect to WPA/WPA2 network:
  WiFi.begin(ssid.c_str(), password.c_str());

  wifi_lasttime = millis();
  if (wifi_delay == 10)
    connexion_try++;
  else
    connexion_try = 1;
  if (WiFi.status() == WL_CONNECTED) {
    IPAddress ip = WiFi.localIP();
    snprintf(localIP, IP_SIZE, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    connexion_try = 0;
  }
  printWifiStatus();
}

void WifiConnexion::connect_delay() {
  unsigned long wifi_start = 0;

  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  // Connect to WPA/WPA2 network:
  wifi_start = millis();
  WiFi.begin(ssid.c_str(), password.c_str());
  while (WiFi.status() != WL_CONNECTED && millis() - wifi_start < 10000)
    delay(10);
  IPAddress ip = WiFi.localIP();
  snprintf(localIP, IP_SIZE, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  printWifiStatus();
}

void WifiConnexion::printWifiStatus() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to wifi");
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(ssid);

    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.print(" dBm (");
    Serial.print(quality());
    Serial.println(")");
  } else {
    Serial.println("Cannot connected to wifi: I'll try later");
  }
}

void WifiConnexion::updateRTC() {
  static unsigned long rtc_lasttime = 0;  //last time rtc update try has been done
  unsigned long epoch               = 0;
  unsigned long rtc_update_start    = 0;

  if (check_time(rtc_lasttime, 3600)) {  // update RTC every 60 min
    title("RTC Update");
    rtc_update_start = millis();
    do {
      epoch = WiFi.getTime();
      delay(1000);
    } while (epoch == 0 && check_time(rtc_update_start, 5));

    if (epoch) {
      Serial.print("Epoch received: ");
      rtc.setEpoch(epoch + timezone_offset * 3600);
      Serial.print(epoch);
      Serial.print(": ");
      printlndatetime();
    } else
      Serial.println("NTP unreachable!!");
    rtc_lasttime = millis();
  }
}

String WifiConnexion::quality() {
  long rssi = WiFi.RSSI();
  unsigned char i;
  long last_rssi = 0;

  for (i = 0; wifi_quality[i].rssi != 0; i++) {
    if (last_rssi >= rssi && rssi >= wifi_quality[i].rssi)
      break;
    last_rssi = wifi_quality[i].rssi;
  }
  return wifi_quality[i].quality;
}

String WifiConnexion::get_ip_as_string() {
  IPAddress ip = WiFi.localIP();
  return String() + ip[0] + "." + ip[1] + "." + ip[2] + "." + ip[3];
}
