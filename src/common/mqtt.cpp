#include "common/mqtt.h"

#include <ArduinoJson.h>

#include "common/helpers.h"
#include "config.h"

void topic_subscriber(String &topic, String &payload) {
  StaticJsonDocument<MQTT_BUFF_SIZE> root;

  if (deserializeJson(root, payload))
    return;  // ERROR
  if (topic == MQTT_TMP_EXT0)
    octoprint.set_ext0(root["actual"], root["target"]);
  else if (topic == MQTT_TMP_EXT1)
    octoprint.set_ext1(root["actual"], root["target"]);
  else if (topic == MQTT_TMP_BED)
    octoprint.set_bed(root["actual"], root["target"]);
#ifdef FS_DEBUG
  else {
    Serial.print("Unkow message arrived in ");
    Serial.println(topic);
    Serial.println(payload);
  }
#endif
}

MQTT::MQTT() {}

void MQTT::begin() {
  title("MQTT Setup");

  mqtt_client.begin(MQTT_HOST, MQTT_PORT, wifi_client);
  connect();
  mqtt_client.onMessage(topic_subscriber);
  mqtt_client.subscribe("#");
}

void MQTT::run() {
  if (!mqtt_client.loop()) {
    if (!mqtt_client.connected())
      connect();
    else
      Serial.println("MQTT ERROR");
  }
}

void MQTT::connect() {
  unsigned long mqtt_start = 0;

  mqtt_start = millis();
  Serial.println("MQTT Connecting");
  while (check_time(mqtt_start, 10) && !mqtt_client.connect("firestone", MQTT_USER, MQTT_PASSWORD))
    delay(100);
  if (mqtt_client.connected())
    Serial.println("connected");
  else
    Serial.println("not connected");
}