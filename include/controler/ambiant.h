#ifndef AMBIANT_RULE_H
#define AMBIANT_RULE_H
#include "rule.h"

class AmbiantRule : public Rule {
 public:
  void begin(){};
  bool proceed(actions_t &actions);

 private:
};
#endif