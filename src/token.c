#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "token.h"
#include "util.h"

// #define DEBUG_ENABLE
#include "debug.h"

const char keywords[] = " true false ";
const char digit[] = "0123456789";
const char id_start[] = "abcdefghikjlmnopqrstuvzwxy_";
const char id[] = "?!-<>=0123456789";
const char punc[] = ",:{}[]";
const char whitespace[] = " \t\n";

token_t current;

static int is_keyword(char *str) {
  int ret;
  ret = (strstr(keywords, str) != NULL);
  DEBUG_PRINT("token->is_keyword %d\n", ret);
  return ret;
};

static int is_digit(char ch) {
  int ret;
  char str[2] = "\0"; /* gives {\0, \0} */
  str[0] = ch;
  ret = (strstr(digit, str) != NULL);
  DEBUG_PRINT("token->is_digit %d\n", ret);
  return ret;
}

static int is_id_start(char ch) {
  int ret;
  // return is_id_start(ch) || "?!-<>=0123456789".indexOf(ch) >= 0;
  char str[2] = "\0"; /* gives {\0, \0} */
  str[0] = ch;
  ret = (strstr(id_start, str) != NULL);
  DEBUG_PRINT("token->is_id_start %d\n", ret);
  return ret;
}

static int is_id(char ch) {
  int ret;
  // return is_id_start(ch) || "?!-<>=0123456789".indexOf(ch) >= 0;
  char str[2] = "\0"; /* gives {\0, \0} */
  str[0] = ch;
  ret = (strstr(id, str) != NULL);
  DEBUG_PRINT("token->is_id %d\n", ret);
  return ret;
}

static int is_punc(char ch) {
  int ret;
  // return ",;(){}[]".indexOf(ch) >= 0;
  char str[2] = "\0"; /* gives {\0, \0} */
  str[0] = ch;
  ret = (strstr(punc, str) != NULL);
  DEBUG_PRINT("token->is_punc %d\n", ret);
  return ret;
}

static int is_whitespace(char ch) {
  int ret;
  char str[2] = "\0"; /* gives {\0, \0} */
  str[0] = ch;
  ret = (strstr(whitespace, str) != NULL);
  DEBUG_PRINT("token->is_whitespace %d\n", ret);
  return ret;
}

static int is_not_eos(char ch) {
  int ret;
  char str[2] = "\0"; /* gives {\0, \0} */
  str[0] = ch;
  ret = (strstr("\"", str) == NULL);
  DEBUG_PRINT("token->is_eos %d\n", ret);
  return ret;
}

static void read_while(char *str, int (*predicate)(char ch)) {
  DEBUG_PRINT("token->read_while\n");
  int i = 0;
  while (!input_eof() && predicate(input_peek())) {
    str[i++] = input_next();
  }
  str[i] = '\0'; // append string terminator
}

static void read_once(char *str, int (*predicate)(char ch)) {
  DEBUG_PRINT("token->read_while\n");
  int i = 0;
  if (!input_eof() && predicate(input_peek())) {
    str[i++] = input_next();
  }
  str[i] = '\0'; // append string terminator
}

static void read_number(void) {
  read_while(current.value, is_digit);
  // return { type: "num", value: parseFloat(number) };
  current.type = token_num;
  DEBUG_PRINT("read_number is: _%s_\n", current.value);
}

static void read_punc(void) {
  read_once(current.value, is_punc);
  // return { type: is_keyword(id) ? "kw" : "var", value: id };
  current.type = token_punc;
  DEBUG_PRINT("read_punc is:  _%s_\n", current.value);
}

static void read_string(void) {
  read_while(current.value, is_not_eos);
  // return { type: "str", value: read_escaped('"') };
  current.type = token_str;
  DEBUG_PRINT("read_string is:  _%s_\n", current.value);
}

static void read_next(void) {
  DEBUG_PRINT("token->read_next\n");
  read_while(current.value, is_whitespace);
  if (input_eof()) {
    current.unempty = 0;
  } else {
    current.unempty = 1;
    char ch = input_peek();
    if (ch == '\"') {
      input_next(); // consume first "
      read_string();
      input_next(); // consume last "
      return;
    } else if (is_digit(ch)) {
      read_number();
      return;
    } else if (is_punc(ch)) {
      // return { type: "punc", value: input.next() };
      read_punc();
      return;
    }
    char str[2] = "\0"; /* gives {\0, \0} */
    str[0] = ch;
    input_croak(str);
  }
}

void token_init(const char *ptr) { input_init(ptr); };

void token_peek(token_t *token) {
  DEBUG_PRINT("token_peek\n");
  if (current.unempty == 0) {
    read_next();
  }
  memcpy(token, &current, sizeof(token_t));
  DEBUG_PRINT("token_peek: %s\n", token->value);
}

void token_next(void) { read_next(); }

void token_croak(char *str) {
  input_croak(str);
  _exit(0);
}

int token_is_eof(void) {
  DEBUG_PRINT("token_eof: %d\n", current.unempty);
  return (current.unempty == 0);
}

int token_is_punc(char *ch) {
  token_t tok;
  int ret = 0;
  token_peek(&tok);
  if ((tok.type == token_punc) && (strcmp(ch, tok.value) == 0)) {
    ret = 1;
  }
  return ret;
}

int token_is_num(void) {
  token_t tok;
  int ret = 0;
  token_peek(&tok);
  // return tok && tok.type == "op" && (!op || tok.value == op) && tok;
  if (tok.type == token_num) {
    ret = 1;
  }
  return ret;
}

int token_is_string(void) {
  token_t tok;
  int ret = 0;
  token_peek(&tok);
  if (tok.type == token_str) {
    ret = 1;
  }
  return ret;
}

void token_skip_punc(char *ch) {
  if (token_is_punc(ch)) {
    token_next();
  } else {
    // input.croak("Expecting punctuation: \"" + ch + "\"");
    token_croak("Expecting punctuation:");
  }
}
