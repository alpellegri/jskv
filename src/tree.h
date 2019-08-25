#ifndef TREE_H
#define TREE_H

#include <stdint.h>

#include "util.h"

typedef enum {
  jsbool, jsint, jsfloat, jsstring, jsarray, jsobject
} jsnode_type;

typedef struct jsnode_s *jsnode;
struct jsnode_s {
  jsnode_type type;
  string key;
  string value;
  jsnode child;
  jsnode next;
};

extern jsnode mkjs_native(jsnode_type type, string k, string v);
extern jsnode mkjs_object(string k, jsnode v);
extern jsnode cpjs(jsnode v);
extern jsnode rmjs_object(jsnode n);
extern void rmjs_object_tree(jsnode root);

extern void jsscan(jsnode node);

extern jsnode setjs(jsnode_type type, jsnode root, string path, string v);
extern jsnode setjs_object(jsnode root, string path, jsnode v);
extern jsnode rmjs(jsnode root, string path);

#endif
