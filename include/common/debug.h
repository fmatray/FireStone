#ifndef DEBUG_H
#define DEBUG_H
#include "config.h"
#ifdef FS_DEBUG
void freememory();

#define PRINT_FILE_LINE()   \
  do {                      \
    Serial.print("[");      \
    Serial.print(__FILE__); \
    Serial.print("::");     \
    Serial.print(__LINE__); \
    Serial.print("] ");     \
  } while (0);

#define DEBUG1(value)      \
  do {                     \
    PRINT_FILE_LINE()      \
    Serial.println(value); \
  } while (0);
#define DEBUG2(key, value) \
  do {                     \
    PRINT_FILE_LINE()      \
    Serial.print(key);     \
    Serial.print(":");     \
    Serial.println(value); \
  } while (0);

#else
#define DEBUG(key, value)
#endif

#endif
