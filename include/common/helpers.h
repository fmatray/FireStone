#ifndef HELPERS_H
#define HELPERS_H
#include <RTCZero.h>

#include "IO/action.h"
#include "common/global.h"
#include "common/status.h"

/*
 *  Serial helpers
 */
void title(String str);
void fail(String message);
void blink(char pin);
/* 
 *  Time helpers 
 */
bool check_time(unsigned long lasttime, unsigned int interval);
bool check_time_ms(unsigned long lasttime, unsigned int interval);

/*
 *  Datetime helpers
 */
String rtc_date();
String rtc_time();
void printdatetime();
void printlndatetime();
void print2digits(short digits);
String twodigits(short digits);

/* 
 * Debounce helper
 */
bool debounce(unsigned long *lasttime, bool *last_state, bool *button_state, const bool reading, const unsigned short interval);

/*
 * Status helpers
 */
String status_str(status_e _status);
String action_str(action_e _action);
#endif
