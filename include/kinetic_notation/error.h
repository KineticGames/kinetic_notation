#ifndef KINETIC_NOTATION_KINETIC_NOTATION_ERROR_H
#define KINETIC_NOTATION_KINETIC_NOTATION_ERROR_H

typedef struct {
  char *message;
} KnError;

const char *kinetic_notation_get_error();

#endif // KINETIC_NOTATION_KINETIC_NOTATION_ERROR_H
