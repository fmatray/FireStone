#include "controler/time.h"

#include "common/global.h"

bool TimeRule::proceed(Actions &actions) {
  if (timer.get_status() == IDLE) {
    actions.display_act   = sleep;
    actions.octoprint_act = sleep;
    actions.buzzer_act    = sleep;
    actions.relay1_act    = sleep;
    actions.relay2_act    = sleep;
    actions.relay3_act    = sleep;
    actions.relay4_act    = sleep;
    actions.message       = "Timer";
  }
  return false;
}