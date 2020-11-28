#ifndef BUZZER_H
#define BUZZER_H
#include "action.h"

class BuzzerAction : public Action {
 public:
  BuzzerAction();
  void buzz(const unsigned short freq);
  void no_buzz();
  void run();
};

#endif