#ifndef UTIL_H
#define UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef char *string;
typedef char bool;

#define TRUE 1
#define FALSE 0

extern char *_strdup(const char *s);
extern void _exit(int v);
extern void *checked_malloc(unsigned int len);
extern void checked_free(void *p);
extern void display_nodes();
extern string String(char *);

#ifdef __cplusplus
}
#endif

#endif /* UTIL_H */
