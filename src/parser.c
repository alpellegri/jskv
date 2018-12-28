#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "token.h"
#include "util.h"

#define DEBUG_ENABLE
#include "debug.h"

int parse(void);

static int parse_key(void) {
  int ret = 0;
  token_t key_tok;
  token_t tok;

  DEBUG_PRINT("parse_key\n");
  token_peek(&key_tok);
  if (token_is_string() == 1) {
    DEBUG_PRINT("parse_key string found\n");
    token_next();
    token_peek(&tok);
    if (token_is_punc(":")) {
      DEBUG_PRINT("parse_key : found\n");
      token_next();
      ret = 1;
    } else {
      DEBUG_PRINT("parse_key : not found\n");
      assert(0);
    }
  } else {
    DEBUG_PRINT("parse_key string not found\n");
    assert(0);
  }

  return ret;
}

static int parse_value(void) {
  int ret = 0;
  token_t tok;

  DEBUG_PRINT("parse_value\n");
  token_peek(&tok);
  if (token_is_punc("{") == 1) {
    DEBUG_PRINT("parse_value json\n");
    ret = parse();
  } else if (token_is_num() == 1) {
    DEBUG_PRINT("parse_value number\n");
    token_next();
    ret = 1;
  } else if (token_is_string() == 1) {
    DEBUG_PRINT("parse_value string\n");
    token_next();
    ret = 1;
  } else {
    assert(0);
  }

  return ret;
}

int parse(void) {
  token_t tok;
  int ret = 0;
  int run = 1;
  int i = 0;
  int key = 0;
  int value = 0;

  DEBUG_PRINT("parse_json\n");
  token_peek(&tok);
  if (token_is_punc("{") == 1) {
    DEBUG_PRINT("parse_json { found\n");
    token_next();
    do {
      DEBUG_PRINT("parse_json [%d]\n", i);
      key = parse_key();
      if (key) {
        value = parse_value();
        if (value) {
          token_peek(&tok);
          if (token_is_punc("}") == 1) {
            DEBUG_PRINT("parse_json } end found\n");
            token_next();
            ret = 1;
            run = 0;
          } else if (token_is_punc(",") == 1) {
            DEBUG_PRINT("parse_json , next found\n");
            token_next();
            run = 1;
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

  return ret;
}

void parse_init(char *ptr) {
  DEBUG_PRINT("parse_init\n");
  /* init token */
  token_init(ptr);
};
