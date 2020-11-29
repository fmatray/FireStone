#include "controler/timer.h"

#include "common/global.h"

bool TimerRule::proceed(Dispatcher &dispatcher) {
  if (timer.get_status() == IDLE)
    dispatcher.set(sleep, sleep, "Timer");
  if (timer.get_status() == OFF)
    dispatcher.set(off, off, "Timer");
  return false;
}