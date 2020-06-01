#ifndef EMERGENCY_RULE_H
#define EMERGENCY_RULE_H
#include "rule.h"

class EmergencyRule : public Rule {
 public:
  void begin(){};
  bool proceed(actions_t &actions);

 private:
};
#endif