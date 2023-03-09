#include "error.h"
#include "kinetic_notation/error.h"

#include <stdlib.h>

typedef struct ErrorNode_t {
  const char *message;
  KnResult associated_result;
  struct ErrorNode_t *next;
} ErrorNode;

static ErrorNode *error_list = NULL;

void add_error(const char *message, KnResult associated_result) {
  ErrorNode *error = malloc(sizeof(ErrorNode));
  error->message = message;
  error->associated_result = associated_result;
  error->next = NULL;

  if (error_list == NULL) {
    error_list = error;
    return;
  }

  ErrorNode *e = error_list;

  while (e->next != NULL) {
    e = e->next;
  }

  e->next = error;
}

const char *kinetic_notation_get_error() {
  if (error_list == NULL) {
    return "No error found!";
  }
  ErrorNode *n = error_list;
  while (n->next != NULL) {
    n = n->next;
  }
  return n->message;
}
