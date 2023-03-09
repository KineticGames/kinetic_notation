#ifndef KINETIC_NOTATION_ERROR_H
#define KINETIC_NOTATION_ERROR_H

#include "result.h"

typedef struct {
  char *message;
  KnResult associated_result;
} KnError;

char *kinetic_notation_get_error();

#endif // KINETIC_NOTATION_ERROR_H
