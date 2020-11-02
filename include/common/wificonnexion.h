#ifndef WIFICONNEXION_H
#define WIFICONNEXION_H
//#include <WiFiMDNSResponder.h>
#include <WiFiNINA.h>

#include "config.h"

#define WIFI_TRIES 5
#define IP_SIZE    16
const struct
{
  String quality;
  long rssi;
} wifi_quality[] = {
    {"Excellent", -30},
    {"Good", -67},
    {"Ok", -70},
    {"Poor", -80},
    {"Unusable", -90},
    {"Error", 0}};

class WifiConnexion {
 public:
  WifiConnexion(const char *_ssid, const char *password);
  void begin();
  void update(uint8_t _timezone_offset) { timezone_offset = _timezone_offset; };
  bool run();
  void connect();
  void connect_delay();
  void printWifiStatus();
  void updateRTC();
  String quality();

  String get_ssid() { return ssid; }
  bool is_connected() { return connected; };
  IPAddress get_ip() { return WiFi.localIP(); };
  String get_ip_as_string();

  char localIP[IP_SIZE];

 private:
//  WiFiMDNSResponder mdns_responder;
  String ssid;
  String password;
  int8_t timezone_offset;
  bool connected;
};
#endif
