#include "error.h"
#include "kinetic_notation/error.h"

#include <stdlib.h>

typedef struct error_node {
  const char *message;
  struct error_node *next;
} error_node;

static error_node *error_list = NULL;

void add_error(const char *message) {
  error_node *error = malloc(sizeof(error_node));
  error->message = message;
  error->next = NULL;

  if (error_list == NULL) {
    error_list = error;
    return;
  }

  error_node *e = error_list;

  while (e->next != NULL) {
    e = e->next;
  }

  e->next = error;
}

const char *kinetic_notation_get_error() {
  if (error_list == NULL) {
    return "No error found!";
  }
  error_node *n = error_list;
  while (n->next != NULL) {
    n = n->next;
  }
  return n->message;
}
