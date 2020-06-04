#ifndef OCTOPRINT_H
#define OCTOPRINT_H
#include <Arduino.h>
#include <OctoPrintAPI.h>
#include <WiFiNINA.h>

class OctoprintBase {
 public:
  void begin();

 protected:
  OctoprintApi *api;
  WiFiClient client;
};

#endif