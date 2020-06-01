#ifndef BUZZER_H
#define BUZZER_H
#include "action.h"

class BuzzerAction : public Action {
 public:
  BuzzerAction(uint8_t _pin, bool _active);
  void buzz(const unsigned short freq);
  void no_buzz();
  void run();

 private:
  uint8_t pin;
  bool active;
};

#endif