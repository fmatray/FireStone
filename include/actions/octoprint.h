#ifndef OCTOPRINT_ACTION_H
#define OCTOPRINT_ACTION_H
#include <Arduino.h>
#include <OctoPrintAPI.h>
#include <WiFiNINA.h>

#include "action.h"
#include "common/octoprint.h"
#include "config.h"

class OctoPrintAction : public OctoprintBase, public Action {
 public:
  void run();
  void reset() { alert_sent = false; };

 private:
  bool alert_sent = false;
};

#endif