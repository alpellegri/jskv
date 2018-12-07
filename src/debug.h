#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#define DEBUG_ENABLE
#ifdef DEBUG_ENABLE
#define DEBUG_PRINT(fmt, ...) do { \
  printf(fmt, ##__VA_ARGS__); \
  fflush(stdout); \
} while(0)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

#endif
