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

  jsnode root;
  jsnode cursor;
  jsnode node;

#if 1
  root = mkjs_native(jsint, "a", "0");
  cursor = root;
  node = mkjs_native(jsint, "b", "1");
  cursor->next = node;
  cursor = cursor->next;
  node = mkjs_native(jsint, "c", "2");
  cursor->next = node;
  cursor = cursor->next;

  jsscan(root);
  DEBUG_PRINT("\n");
  rmjs_object_tree(root);
#endif

#if 1
  root = mkjs_object("root", NULL);
  cursor = root;
  node = mkjs_native(jsint, "a", "0");
  cursor->child = node;
  cursor = cursor->child;
  node = mkjs_native(jsint, "b", "1");
  cursor->next = node;

  jsscan(root);
  DEBUG_PRINT("\n");
  rmjs_object_tree(root);
#endif

#if 1
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

#if 0 // no
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

#if 0 // no
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

  rmjs_object_tree(json);
  rmjs_object_tree(json2);
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
  root = parse();
#if 0
  root = setjs(jsint, root, "d/m", "10");
  root = setjs(jsint, root, "d/l", "20");
  root = setjs(jsint, root, "d/m", "30");
  root = setjs(jsint, root, "e", "end");
#endif
  jsscan(root);
  DEBUG_PRINT("\n");
  rmjs_object_tree(root);
#endif

  display_nodes();
  return 0;
}
