#ifndef EMERGENCY_RULE_H
#define EMERGENCY_RULE_H
#include "rule.h"

class EmergencyRule : public Rule {
 public:
  void begin(){};
  bool proceed(Dispatcher &dispatcher);

 private:
};
#endif