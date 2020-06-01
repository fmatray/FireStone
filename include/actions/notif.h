#ifndef NOTIF_H
#define NOTIF_H
#include "action.h"
#include "status.h"


class NotifAction : Action {
 public:
  NotifAction(status_e stat, char *msg);

 private:
  status_e status;
  char *message;
};

#endif
