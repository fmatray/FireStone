#ifndef OCTOPRINT_RULE_H
#define OCTOPRINT_RULE_H
#include "rule.h"

class OctoprintRule : public Rule {
 public:
  void begin(){};
  bool proceed(Dispatcher &dispatcher);

 private:
};
#endif