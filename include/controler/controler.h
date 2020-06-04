#ifndef CONTROLER_H
#define CONTROLER_H

#include "rule.h"
#include "common/status.h"

#define RULES_SIZE     10
#define RESET_INTERVAL 1000  // 2s debounce to avoid false manipulation

class Controler {
 public:
  Controler(const uint8_t _reset_pin);
  void begin();
  void reset();
  void run();
  void dispatch(const actions_t actions);

  /* Helpers*/
  void print_actions(const actions_t actions);

 private:
  Rule *rules[RULES_SIZE];
  actions_t actions;
  uint8_t reset_pin;
};
#endif