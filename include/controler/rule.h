#ifndef RULE_H
#define RULE_H
#include <Arduino.h>

#include "actions/action.h"
#include "common/status.h"

class Actions {
 public:
  bool operator!=(const Actions &a) {
    return display != a.display ||
           buzzer != a.buzzer ||
           octoprint != a.octoprint ||
           relay1 != a.relay1 ||
           relay2 != a.relay2 ||
           relay3 != a.relay3 ||
           relay4 != a.relay4 ||
           message != a.message;
  };
  action_e display;
  action_e buzzer;
  action_e octoprint;
  action_e relay1;
  action_e relay2;
  action_e relay3;
  action_e relay4;

  String message;
};

class Rule {
 public:
  virtual void begin()                   = 0;
  virtual bool proceed(Actions &actions) = 0;
  bool is_active() { return active; };

 private:
  bool active = true;
};
#endif