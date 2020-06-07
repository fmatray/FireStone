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
  bool is_off() { return general == off; };
  bool is_off_or_sleeping() { return is_off() || is_sleeping(); };
  void reset_or_wakeup();

  void set(const action_e _general, const action_e _buzz, const String _msg);
  inline void update_message(const String _msg) { message = _msg; };

 private:
  action_e general;
  action_e buzz;
  String message;

  action_e last_general;
  action_e last_buzz;
};
#endif