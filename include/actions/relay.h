#ifndef RELAY_H
#define RELAY_H
#include "action.h"

typedef struct {
  bool inverted;
  action_e switch_status;
} relay_settings_t;

class RelayAction : public Action {
 public:
  RelayAction(uint8_t _pin);
  void update(const relay_settings_t _settings);
  void run();
  bool get_state();
  String get_state_as_str();
  void turn_on();
  void turn_off();

 private:
  uint8_t pin;
  bool state;
  bool inverted;
  action_e switch_status;
};

#endif