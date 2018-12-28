#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef DEBUG_ENABLE
#define DEBUG_PRINT(fmt, ...) do { \
  printf(fmt, ##__VA_ARGS__); \
  fflush(stdout); \
} while(0)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

#endif
