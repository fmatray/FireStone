#ifndef ACTION_H
#define ACTION_H
#include <Arduino.h>

#include "common/status.h"

enum action_e {
  nothing,
  sleep,
  wakeup,
  err,
  warn,
  alert
};

class Action {
 public:
  virtual void run() = 0;
  void set(action_e _status, String _message = "") {
    action  = _status;
    message = _message;
  };

 protected:
  action_e action = nothing;
  String message  = "";
};

#endif
