#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"

// #define DEBUG
#ifdef DEBUG
#define debug_printf(fmt, args...) printf(fmt, ##args)
#else
#define debug_printf(fmt, args...) /* Don't do anything in release builds */
#endif

const char *input_input = NULL;
int input_pos = 0;
int input_line = 1;
int input_col = 0;

void input_init(const char *ptr) { input_input = ptr; }

char input_charAt(int pos) {
  debug_printf("input->charAt %d\n", pos);
  return input_input[pos];
}

char input_next(void) {
  debug_printf("input->next %d\n", pos);
  char ch = input_charAt(input_pos++);
  if (ch == '\n') {
    input_line++, input_col = 0;
  } else {
    input_col++;
  }
  return ch;
}

char input_peek(void) {
  debug_printf("input->peek %d\n", pos);
  return input_charAt(input_pos);
}

int input_eof(void) {
  debug_printf("input->eof %d\n", input_pos);
  return input_peek() == '\0';
}

void input_croak(char *str) { printf("input croak char: %s \n", str); }
