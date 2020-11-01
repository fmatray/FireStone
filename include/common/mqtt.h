#ifndef MQTT_H
#define MQTT_H

#include <MQTTClient.h>
#include <WiFiNINA.h>

#define MQTT_BUFF_SIZE 128

#define MQTT_TMP_EXT0 "octoPrint/temperature/tool0"
#define MQTT_TMP_EXT1 "octoPrint/temperature/tool1"
#define MQTT_TMP_BED  "octoPrint/temperature/bed"

class MQTT {
 public:
  MQTT();
  void begin();
  void run();
  bool connected() { return wifi_client.connected() && mqtt_client.connected(); };

 private:
  WiFiClient wifi_client;
  MQTTClient mqtt_client;

  void connect();
};

#endif