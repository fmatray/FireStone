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
};
#endif