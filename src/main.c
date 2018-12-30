#include <stdint.h>
#include <string.h>

#include "parser.h"
#include "token.h"
#include "tree.h"
#include "util.h"

#define DEBUG_ENABLE
#include "debug.h"

int main(void) {
  DEBUG_PRINT("main\n");

#if 0
  jsnode root = mkjs_native(jsint, "a", "0");
  jsnode cursor = root;
  //cursor = cursor->child;
  jsnode node = mkjs_native(jsint, "b", "1");
  cursor->next = node;

  jsscan(root);
  DEBUG_PRINT("\n");
  rmjs_object(root);
#endif

#if 0
  jsnode root = mkjs_object("root", NULL);
  jsnode cursor = root;
  jsnode node = mkjs_native(jsint, "a", "0");
  cursor->child = node;
  cursor = cursor->child;
  node = mkjs_native(jsint, "b", "1");
  cursor->next = node;

  jsscan(root);
  DEBUG_PRINT("\n");
  rmjs_object(root);
#endif

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

#if 1
  static const char *JSON_STRING = //
      "{"                          //
      "\"a\":true,"                //
      "\"b\":\"1\","               //
      "\"c\":\"2\","               //
      "\"d\":"                     //
      "{"                          //
      "\"e\":0,"                   //
      "\"f\":\"1\","               //
      "\"g\":\"2\","               //
      "\"h\":\"3\""                //
      "}"                          //
      "}";                         //

  parse_init(JSON_STRING);
  jsnode node = parse();
  jsscan(node);
  node = setjs(jsint, node, "d/m", "10");
  node = setjs(jsint, node, "d/l", "20");
  node = setjs(jsint, node, "d/m", "30");
  node = setjs(jsint, node, "e", "end");
  jsscan(node);
  rmjs_object(node);
#endif

  return 0;
}
