#ifndef FIRE_RULE_H
#define FIRE_RULE_H
#include "rule.h"

class FireRule : public Rule {
 public:
  void begin(){};
  bool proceed(actions_t &actions);

 private:
};
#endif