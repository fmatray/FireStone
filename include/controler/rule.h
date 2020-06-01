#ifndef RULE_H
#define RULE_H
#include <Arduino.h>

#include "status.h"

typedef struct {
  status_e global;
  status_e buzzer;
  
  status_e octoprint;
  status_e relay1;
  status_e relay2;
  status_e relay3;
  status_e relay4;

  String message;
} actions_t;

class Rule {
 public:
  virtual void begin()                     = 0;
  virtual bool proceed(actions_t &actions) = 0;
  bool is_active() { return active; };

 private:
  bool active = true;
};
#endif