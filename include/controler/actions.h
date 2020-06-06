#ifndef ACTIONS_H
#define ACTIONS_H
#include "IO/action.h"
#include "common/status.h"

class Actions {
 public:
  action_e display_act;
  action_e buzzer_act;
  action_e octoprint_act;
  action_e relay1_act;
  action_e relay2_act;
  action_e relay3_act;
  action_e relay4_act;

  String message;

  bool operator!=(const Actions &a);
  void reset();
  bool need_reset();
  void dispatch();
  void print();
};
#endif