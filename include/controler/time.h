#ifndef TIME_RULE_H
#define TIME_RULE_H
#include "rule.h"

class TimeRule : public Rule {
 public:
  void begin(){};
  bool proceed(Actions &actions);

 private:
};
#endif