#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "token.h"
#include "tree.h"
#include "util.h"

#define DEBUG_ENABLE
#include "debug.h"

jsnode parse(void);

static char *parse_key(void) {
  int ret = 0;
  token_t tok;
  char *key = NULL;
  char *str;

  DEBUG_PRINT("parse_key\n");
  token_peek(&tok);
  if (token_is_string() == 1) {
    str = String(tok.value);
    DEBUG_PRINT("parse_key string found %s\n", str);
    token_next();
    token_peek(&tok);
    if (token_is_punc(":")) {
      key = str;
      DEBUG_PRINT("parse_key : found\n");
      token_next();
    } else {
      DEBUG_PRINT("parse_key : not found\n");
      assert(0);
    }
  } else {
    DEBUG_PRINT("parse_key string not found\n");
    assert(0);
  }

  return key;
}

jsnode parse_value(char *key) {
  jsnode node = NULL;
  token_t tok;

  DEBUG_PRINT("parse_value\n");
  token_peek(&tok);
  if (token_is_punc("{") == 1) {
    DEBUG_PRINT("parse_value json\n");
    jsnode _node = parse();
    node = mkjs_object(key, _node);
  } else if (token_is_num() == 1) {
    DEBUG_PRINT("parse_value number\n");
    node = mkjs_native(jsint, key, tok.value);
    token_next();
  } else if (token_is_string() == 1) {
    DEBUG_PRINT("parse_value string\n");
    node = mkjs_native(jsstring, key, tok.value);
    token_next();
  } else if ((token_is_kw("true") == 1) || (token_is_kw("false") == 1)) {
    DEBUG_PRINT("parse_value string\n");
    node = mkjs_native(jsbool, key, tok.value);
    token_next();
  } else {
    assert(0);
  }

  return node;
}

jsnode parse(void) {
  token_t tok;
  jsnode node = NULL;
  int run = 1;
  int i = 0;
  char *key;

  jsnode root = NULL;
  jsnode cursor;

  DEBUG_PRINT("parse_json\n");
  token_peek(&tok);
  if (token_is_punc("{") == 1) {
    DEBUG_PRINT("parse_json { found\n");
    token_next();
    do {
      DEBUG_PRINT("parse_json [%d]\n", i);
      key = parse_key();
      if (key != NULL) {
        DEBUG_PRINT("key _%s_\n", key);
        node = parse_value(key);
        // free the key
        checked_free(key);
        if (node != NULL) {
          if (root == NULL) {
            root = node;
            cursor = root;
          } else {
            cursor->next = node;
            cursor = cursor->next;
          }
          token_peek(&tok);
          if (token_is_punc("}") == 1) {
            DEBUG_PRINT("parse_json } end found\n");
            token_next();
            run = 0;
          } else if (token_is_punc(",") == 1) {
            DEBUG_PRINT("parse_json , next found\n");
            token_next();
          } else {
            assert(0);
          }
        } else {
          assert(0);
        }
      } else {
        assert(0);
      }
      i++;
    } while (run == 1);
  } else {
    DEBUG_PRINT("parse_json { not found\n");
    assert(0);
  }

  return root;
}

void parse_init(const char *ptr) {
  DEBUG_PRINT("parse_init\n");
  /* init token */
  token_init(ptr);
};
