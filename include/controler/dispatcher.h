#ifndef DISPATCHER_H
#define DISPATCHER_H
#include "IO/action.h"
#include "common/status.h"

class Dispatcher {
 public:
  void reset();
  bool need_reset();
  void dispatch();
  void print();

  bool is_sleeping() { return general == sleep; };
  void reset_or_wakeup();

  inline void set(const action_e _general, const action_e _buzz, const String _msg) {
    general = _general;
    buzz    = _buzz;
    message = _msg;
  }
  inline void update_message(const String _msg) { message = _msg; };

 private:
  action_e general;
  action_e buzz;
  String message;

  action_e last_general;
  action_e last_buzz;
};
#endif