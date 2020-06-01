#ifndef SENSOR_H
#define SENSOR_H
#include "status.h"

class Sensor {
 public:
  virtual void begin();
  virtual bool test();
  void reset() { status = OK; };
  virtual void run();
  virtual bool read();
  virtual status_e check() { return NO_CHECK; }
  status_e get_status() { return status; };

 protected:
  status_e status;
};

#endif
