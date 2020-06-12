#ifndef MQTT_H
#define MQTT_H
#include <MQTTClient.h>
#include <WiFiNINA.h>

class MQTT {
 public:
  MQTT();
  void begin();
  void run();

 private:
  WiFiClient wifi_client;
  MQTTClient mqtt_client;
};

#endif