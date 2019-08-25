#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef DEBUG_ENABLE
#define DEBUG_PRINT(fmt, args...) do { \
  printf(fmt, ##args); \
} while(0)
#else
#define DEBUG_PRINT(fmt, args...)
#endif

#endif
