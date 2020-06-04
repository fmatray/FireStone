#include "common/debug.h"

#include <Arduino.h>

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char *sbrk(int incr);
#else   // __ARM__
extern char *__brkval;
#endif  // __arm__

void freememory() {
  char top;

  Serial.print("Free memory:");
#ifdef __arm__
  Serial.println(&top - reinterpret_cast<char *>(sbrk(0)));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  Serial.println(&top - __brkval);
#else   // __arm__
  Serial.println(__brkval ? &top - __brkval : &top - __malloc_heap_start);
#endif  // __arm__
}