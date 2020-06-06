#include "controler/time.h"

#include "common/global.h"

bool TimeRule::proceed(Dispatcher &dispatcher) {
  if (timer.get_status() == IDLE)
    dispatcher.set(sleep, sleep, "Timer");
  if (timer.get_status() == OFF)
    dispatcher.set(off, off, "Timer");
  return false;
}