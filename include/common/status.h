#ifndef STATUS_H
#define STATUS_H

/* Check */
enum status_e {
  NO_CHECK = -2,  // No testing
  OFF      = -1,  // Sensor is OFF
  IDLE     = 0,   // Value is also OK, item is waiting
  OK       = 1,   // Value is OK
  ERROR    = 2,   // Error while testing
  WARNING  = 3,   // 95% of the max value is reached
  ALERT    = 4    // Max value is passed
};

#endif