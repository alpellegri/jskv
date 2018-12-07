/*
 * Commonly used utility functions.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

// #define DEBUG
#ifdef DEBUG
#define debug_printf(fmt, args...)                                             \
  do {                                                                         \
    printf(fmt, ##args);                                                       \
    fflush(stdout);                                                            \
  } while (0)
#else
#define debug_printf(fmt, args...) /* Don't do anything in release builds */
#endif

typedef struct node_s *node_p;
struct node_s {
  unsigned int key;
  unsigned int data;
  node_p next;
} node_t;

static node_p node_head = NULL;
static node_p node_curr = NULL;

// display the list
void display_nodes() {
  node_p ptr = node_head;
  unsigned int mem = 0;

  // printf("display_nodes:\n");
  // start from the beginning
  while (ptr != NULL) {
    mem += ptr->data;
    // printf("(%x,%d)\n", ptr->key, ptr->data);
    ptr = ptr->next;
  }
  debug_printf("display_nodes memory: %d\n", mem);
}

// insert link at the first location
void mk_node(unsigned int key, unsigned int data) {
  // create a link
  node_p link = (node_p)malloc(sizeof(node_t));
  link->key = key;
  link->data = data;
  // point it to old first node
  link->next = node_head;
  // point first to new first node
  node_head = link;
}

// remove a link with given key
node_p rm_node(unsigned int key) {
  // start from the first link
  node_p curr = node_head;
  node_p prev = NULL;

  // if list is empty
  if (node_head == NULL) {
    printf("rm_node: NULL node!\n");
    return NULL;
  }

  // navigate through list
  while (curr->key != key) {
    // if it is last node
    if (curr->next == NULL) {
      return NULL;
    } else {
      // store reference to curr link
      prev = curr;
      // move to next link
      curr = curr->next;
    }
  }

  // found a match, update the link
  if (curr == node_head) {
    // change first to point to next link
    node_head = node_head->next;
  } else {
    // bypass the curr link
    prev->next = curr->next;
  }

  return curr;
}

/* helper function */
char *_strdup(const char *s) {
  debug_printf("_strdup\n");
  char *d = checked_malloc(strlen(s) + 1); // Space for length plus nul
  if (d == NULL) {
    return NULL; // No memory
  }
  strcpy(d, s); // Copy the characters
  return d;     // Return the new string
}

/* helper function */
void _exit(int v) { exit(v); }

void *checked_malloc(unsigned int len) {
  void *p = calloc(len, 1);
  if (!p) {
    printf("Ran out of memory!\n");
    _exit(1);
  } else {
#ifdef DEBUG
    debug_printf("checked_malloc %x, %d\n", (unsigned int)p, len);
    mk_node((unsigned int)p, len);
    display_nodes();
#endif
  }
  return p;
}

void checked_free(void *p) {
  if (p == 0) {
    printf("checked_free error\n");
    _exit(1);
  } else {
#ifdef DEBUG
    node_p node;
    node = rm_node((unsigned int)p);
    debug_printf("checked_free %x,%d\n", (unsigned int)p, node->data);
    if (node != 0) {
      free(node);
    } else {
      debug_printf("checked_free: node not found!\n");
    }
    display_nodes();
#endif
    free(p);
  }
}

string String(char *s) {
  string p = checked_malloc(strlen(s) + 1);
  strcpy(p, s);
  return p;
}
