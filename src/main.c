#include <stdint.h>
#include <string.h>

#include "parser.h"
#include "token.h"
#include "tree.h"
#include "util.h"

#define DEBUG_ENABLE
#include "debug.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

#if 0
static const char *JSON_STRING =
    "{\"user\":\"johndoe\",\"admin\":false,\"uid\":1000,"
    "\"groups\":[\"users\",\"wheel\",\"audio\",\"video\"]}";
#else
static char *JSON_STRING = "{\"a\":0,\"b\":\"ale\"}";
#endif

int main(void) {
  DEBUG_PRINT("main\n");
  parse_init(JSON_STRING);
  parse();

#if 0
  jsnode json = NULL;
  jsnode json2 = NULL;

  DEBUG_PRINT("test1:\n");
  json = setjs(jsint, json, "a1/b1/c1", "10");
  DEBUG_PRINT("jsscan1:\n");
  jsscan(json);
  DEBUG_PRINT("\n");

  DEBUG_PRINT("test2:\n");
  json = setjs(jsint, json, "a1/b1/c2", "20");
  DEBUG_PRINT("jsscan2:\n");
  jsscan(json);
  DEBUG_PRINT("\n");

  DEBUG_PRINT("test3:\n");
  json = setjs(jsint, json, "a1/b1/c2", "30");
  DEBUG_PRINT("jsscan3:\n");
  jsscan(json);
  DEBUG_PRINT("\n");

  DEBUG_PRINT("test4:\n");
  json = setjs(jsint, json, "a1/b1/c1", "30");
  DEBUG_PRINT("jsscan4:\n");
  jsscan(json);
  DEBUG_PRINT("\n");

#if 0
  DEBUG_PRINT("test5:\n");
  json = rmjs(json, "a1/b1/c1");
  DEBUG_PRINT("jsscan5:\n");
  jsscan(json);
  DEBUG_PRINT("\n");
#endif

  DEBUG_PRINT("test6:\n");
  json2 = cpjs(json);
  DEBUG_PRINT("jsscan6:\n");
  jsscan(json2);
  DEBUG_PRINT("\n");

#if 0
  DEBUG_PRINT("test7:\n");
  json = rmjs(json, "a1/b1/c2");
  DEBUG_PRINT("jsscan7:\n");
  jsscan(json);
  DEBUG_PRINT("\n");
#endif

  DEBUG_PRINT("test8:\n");
  json = setjs(jsint, json, "a1/b1/c2", "10");
  DEBUG_PRINT("jsscan8:\n");
  jsscan(json);
  DEBUG_PRINT("\n");

  DEBUG_PRINT("test9:\n");
  json = setjs_object(json, "a1/b1/c1", json2);
  DEBUG_PRINT("jsscan9:\n");
  jsscan(json);
  DEBUG_PRINT("\n");

  rmjs_object(json);
#endif

  return 0;
}
