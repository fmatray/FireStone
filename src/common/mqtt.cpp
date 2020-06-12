#include "common/mqtt.h"

#include "common/helpers.h"
#include "config.h"

void topic_subscriber(String &topic, String &payload) {
  Serial.print("Message arrived in ");
  Serial.println(topic);
  Serial.println(payload);
}

MQTT::MQTT() {}

void MQTT::begin() {
  title("MQTT Setup");

  mqtt_client.begin(MQTT_HOST, MQTT_PORT, wifi_client);
  while (!mqtt_client.connect("firestone", MQTT_USER, MQTT_PASSWORD))
    delay(100);
  Serial.println("connected");
  mqtt_client.onMessage(topic_subscriber);
  mqtt_client.subscribe("#");
}

void MQTT::run() {
  if (!mqtt_client.loop()) {
    if (!mqtt_client.connected()) {
      Serial.println("MQTT Connecting");
      while (!mqtt_client.connect("firestone", MQTT_USER, MQTT_PASSWORD))
        delay(100);
      Serial.println("MQTT done");
    } else
      Serial.println("MQTT ERROR");
  }
}