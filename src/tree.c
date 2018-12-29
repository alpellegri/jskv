#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"
#include "util.h"

#define DEBUG_ENABLE
#include "debug.h"

/* Left-child right-sibling binary tree, where
   not leaf node child contains a path
   leaf node child is null

          1
         /
        2---3---4
       /       /
      5---6   7
             /
           8---9*


          1
         /
        2
       / \
      5   3
       \   \
        6   4
           /
          7
         /
        8
         \
          9*

  * parent is 7
    prev is 8
    node is 9
    cursor is null
*/

jsnode mkjs_native(jsnode_type type, string k, string v) {
  // DEBUG_PRINT("> mkjs_native %s %s\n", k, v);
  jsnode node = checked_malloc(sizeof *node);
  node->type = jsstring;
  node->key = String(k);
  node->value = String(v);
  node->child = NULL;
  node->next = NULL;
  return node;
}

jsnode mkjs_object(string k, jsnode v) {
  // DEBUG_PRINT("> mkjs_object %s\n", k);
  jsnode node = checked_malloc(sizeof *node);
  node->type = jsobject;
  node->key = String(k);
  node->value = NULL;
  node->child = v;
  node->next = NULL;
  return node;
}

jsnode cpjs(jsnode v) {
  static uint8_t level;
  level++;
  jsnode root;

  // DEBUG_PRINT("> cpjs %d\n", level);
  if (v->value) {
    assert((v->type == jsbool) || (v->type == jsint) || (v->type == jsfloat) ||
           (v->type == jsstring));
    assert(v->child == NULL);
    root = mkjs_native(v->type, v->key, v->value);
    if (v->next) {
      root->next = cpjs(v->next);
    }
  } else {
    assert(v->value == NULL);
    assert((v->type == jsarray) || (v->type == jsobject));
    // assert(v->child != NULL);
    jsnode copy = NULL;
    if (v->child) {
      copy = cpjs(v->child);
    }
    root = mkjs_object(v->key, copy);
    if (v->next) {
      root->next = cpjs(v->next);
    }
  }

  level--;
  return root;
}

jsnode rmjs_object(jsnode root) {
  static uint8_t level;
  level++;
  jsnode ret;

  // DEBUG_PRINT("> rmjs_object %d\n", level);
  if (root != NULL) {
    // DEBUG_PRINT("rmjs_object %s\n", root->key);
    ret = root->next;
    if (root->value) {
      assert(root->child == NULL);
      assert((root->type == jsbool) || (root->type == jsint) ||
             (root->type == jsfloat) || (root->type == jsstring));
      checked_free(root->value);
      checked_free(root->key);
    } else {
      assert((root->type == jsarray) || (root->type == jsobject));
      if (root->child != NULL) {
        rmjs_object(root->child);
      }
      checked_free(root->key);
    }
    checked_free(root);
  } else {
    DEBUG_PRINT("rmjs_object empty\n", root);
  }

  level--;
  return ret;
}

string pathfind(jsnode root, string *path, jsnode *parent, jsnode *prev,
                jsnode *node) {
  char *p;
  char *key = NULL;
  jsnode cursor = root;

  do {
    if (key != NULL) {
      checked_free(key);
    }
    /* get current path-key */
    DEBUG_PRINT("---------------------- get path-key %s\n", *path);
    p = strstr(*path, "/");
    if (p != NULL) {
      char len = p - *path;
      key = checked_malloc(len + 1);
      memcpy(key, *path, len);
      /* advance path */
      *path = &(*path)[len + 1];
    } else {
      key = String(*path);
      *path = NULL;
    }
    /* search node */
    *parent = *node;
    *prev = NULL;
    *node = NULL;
    while ((*node == NULL) && (cursor != NULL)) {
      // DEBUG_PRINT("strcmp %s %s\n", cursor->key, key);
      if (strcmp(cursor->key, key) == 0) {
        // DEBUG_PRINT("found %s %s\n", cursor->key, key);
        *node = cursor;
        cursor = cursor->child;
      } else {
        *prev = cursor;
        cursor = cursor->next;
      }
    }
    // DEBUG_PRINT("while %x %x\n", p, cursor);
  } while ((p != NULL) && (cursor != NULL));

  return key;
}

jsnode setjs(jsnode_type type, jsnode root, string path, string v) {
  static uint8_t level;
  level++;
  char *p;
  char *key = NULL;
  jsnode node;
  jsnode prev;
  jsnode parent;

  DEBUG_PRINT("> setjs %d\n", level);
  key = pathfind(root, &path, &parent, &prev, &node);

  /*  */
  if (node != NULL) {
    DEBUG_PRINT("setjs-root not empty %s\n", node->key);
    jsnode next = node->next;
    rmjs_object(node);
    node = mkjs_native(type, key, v);
    if (prev == NULL) {
      parent->child = node;
    } else {
      prev->next = node;
    }
  } else {
    // create an objs cascade
    /* empty */
    // DEBUG_PRINT("setjs-root empty %s\n", key);
    if (path != NULL) {
      // DEBUG_PRINT("setjs-root is not a leaf %s\n", key);
      /* this is not a leaf */
      jsnode node = setjs(type, NULL, path, v);
      if (prev == NULL) {
        // DEBUG_PRINT("setjs-root null empty %s\n", key);
        root = mkjs_object(key, node);
      } else {
        prev->next = mkjs_object(key, node);
      }
    } else {
      // DEBUG_PRINT("setjs-root is a leaf %s\n", key);
      /* this is a leaf */
      if (prev == NULL) {
        root = mkjs_native(type, key, v);
      } else {
        prev->next = mkjs_native(type, key, v);
      }
    }
  }

  if (key != NULL) {
    checked_free(key);
  }

  level--;
  return root;
}

jsnode setjs_object(jsnode root, string path, jsnode v) {
  static uint8_t level;
  level++;
  char *key = NULL;
  jsnode node;
  jsnode prev;
  jsnode parent;

  DEBUG_PRINT("> setjs_object %d\n", level);
  key = pathfind(root, &path, &parent, &prev, &node);

  /*  */
  if (node != NULL) {
    // DEBUG_PRINT("setjs_object-root not empty %s\n", node->key);
    jsnode next = node->next;
    rmjs_object(node);
    jsnode copy = cpjs(v);
    node = mkjs_object(key, copy);
    if (prev == NULL) {
      parent->child = node;
    } else {
      prev->next = node;
    }
  } else {
    // create an objs cascade
    /* empty */
    // DEBUG_PRINT("setjs_object-root empty %s\n", key);
    if (path != NULL) {
      // DEBUG_PRINT("setjs_object-root is not a leaf %s\n");
      /* this is not a leaf */
      jsnode node = setjs_object(NULL, path, v);
      if (prev == NULL) {
        // DEBUG_PRINT("setjs_object-root null empty %s\n", key);
        root = mkjs_object(key, node);
      } else {
        // DEBUG_PRINT("setjs_object-root next empty %s\n", key);
        prev->next = mkjs_object(key, node);
      }
    } else {
      // DEBUG_PRINT("setjs_object-root is a leaf %s\n", key);
      jsnode copy = cpjs(v);
      /* this is a leaf */
      if (prev == NULL) {
        root = copy;
      } else {
        prev->next = copy;
      }
    }
  }

  if (key != NULL) {
    checked_free(key);
  }

  level--;
  return root;
}

jsnode rmjs(jsnode root, string path) {
  static uint8_t level;
  level++;
  char *key = NULL;
  jsnode node;
  jsnode prev;
  jsnode parent;

  // DEBUG_PRINT("> rmjs %d\n", level);
  key = pathfind(root, &path, &parent, &prev, &node);

  /*  */
  if (node != NULL) {
    // is a child
    // DEBUG_PRINT("rmjs-root not empty %s\n", node->key);
    if (prev != NULL) {
      // DEBUG_PRINT("rmjs-root previous new link %s\n", prev->key);
      prev->next = rmjs_object(node);
    } else {
      assert(parent != NULL);
      // DEBUG_PRINT("rmjs-root parent new link %s\n", parent->key);
      parent->child = rmjs_object(node);
    }
  } else {
    // not found
    // DEBUG_PRINT("rmjs-root not found %s\n", node->key);
  }

  if (key != NULL) {
    checked_free(key);
  }

  return root;
}

void jsscan(jsnode n) {
  static uint8_t level;
  level++;
  char str[25];
  int i;
  if (n != NULL) {
    for (i = 0; i < 2 * level; i++) {
      str[i] = ' ';
    }
    str[i] = '\0';
    if (n->value) {
      assert(n->child == NULL);
      DEBUG_PRINT("%s %s %s\n", str, n->key, n->value);
      level--;
      if (n->next) {
        jsscan(n->next);
      }
    } else {
      // assert(n->child != NULL);
      DEBUG_PRINT("%s %s\n", str, n->key);
      if (n->child) {
        jsscan(n->child);
      }
      level--;
      if (n->next) {
        jsscan(n->next);
      }
    }
  }
}
