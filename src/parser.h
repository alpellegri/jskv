#ifndef PARSER_H
#define PARSER_H

#include "tree.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void parse_init(const char *ptr);
extern jsnode parse(void);

#ifdef __cplusplus
}
#endif

#endif /* PARSER_H */
