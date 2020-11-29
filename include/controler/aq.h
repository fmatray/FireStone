#ifndef AIRQUALITY_RULE_H
#define AIRQUALITY_RULE_H
#include "rule.h"

class AirQualityRule : public Rule {
 public:
  void begin(){};
  bool proceed(Dispatcher &dispatcher);

 private:
};
#endif