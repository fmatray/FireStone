#ifndef CONTROLER_H
#define CONTROLER_H

#include "common/status.h"
#include "dispatcher.h"
#include "rule.h"

#define RULES_SIZE     10
#define RESET_INTERVAL 1000  // 2s debounce to avoid false manipulation

class Controler {
 public:
  void begin();
  void reset();
  void run();

 private:
  Rule *rules[RULES_SIZE];
  Dispatcher dispatcher;
};
#endif