#ifndef SENSOR_H
#define SENSOR_H
#include "common/status.h"

class Sensor {
 public:
  virtual void begin() = 0;
  virtual bool test()  = 0;
  void reset() { status = OK; };
  virtual void run()  = 0;
  virtual bool read() = 0;
  virtual status_e check() { return NO_CHECK; }
  status_e get_status() { return status; };

 protected:
  status_e status;
};

#endif
