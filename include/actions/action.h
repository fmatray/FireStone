#ifndef ACTION_H
#define ACTION_H
#include <Arduino.h>
#include "status.h"

class Action {
 public:
  virtual void run() = 0;
  void set_status(status_e _status, String _message = "") {
    status  = _status;
    message = _message;
  };

 protected:
  status_e status = IDLE;
  String message  = "";
};

#endif
