#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "token.h"
#include "util.h"

#define DEBUG
#ifdef DEBUG
#define debug_printf(fmt, args...) printf(fmt, ##args)
#else
#define debug_printf(fmt, args...) /* Don't do anything in release builds */
#endif

int parse(void);

static int parse_key(void) {
  int ret = 0;
  token_t key_tok;
  token_t tok;

  debug_printf("parse_key\n");
  token_peek(&key_tok);
  if (token_is_string() == 1) {
    debug_printf("parse_key string found\n");
    token_next();
    token_peek(&tok);
    if (token_is_punc(":")) {
      debug_printf("parse_key : found\n");
      token_next();
      ret = 1;
    } else {
      assert(0);
    }
  } else {
    assert(0);
  }

  return ret;
}

static int parse_value(void) {
  int ret = 0;
  token_t tok;

  debug_printf("parse_value\n");
  token_peek(&tok);
  if (token_is_punc("{") == 1) {
    debug_printf("parse_value json\n");
    ret = parse();
  } else if (token_is_num() == 1) {
    debug_printf("parse_value number\n");
    token_next();
    ret = 1;
  } else if (token_is_string() == 1) {
    debug_printf("parse_value string\n");
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

  debug_printf("parse_json\n");
  token_peek(&tok);
  if (token_is_punc("{") == 1) {
    debug_printf("parse_json { found\n");
    token_next();
    do {
      debug_printf("parse_json [%d]\n", i);
      key = parse_key();
      if (key) {
        value = parse_value();
        if (value) {
          token_peek(&tok);
          if (token_is_punc("}") == 1) {
            debug_printf("parse_json } found\n");
            token_next();
            ret = 1;
            run = 0;
          } else if (token_is_punc(",") == 1) {
            debug_printf("parse_json , found\n");
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
    assert(0);
  }

  return ret;
}

void parse_init(char *ptr) {
  debug_printf("parse_init\n");
  /* init token */
  token_init(ptr);
};
