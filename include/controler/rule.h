#ifndef RULE_H
#define RULE_H
#include <Arduino.h>

#include "controler/dispatcher.h"

class Rule {
 public:
  virtual void begin()                         = 0;
  virtual bool proceed(Dispatcher &dispatcher) = 0;
  bool is_active() { return active; };

 private:
  bool active = true;
};
#endif