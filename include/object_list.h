#ifndef KINETIC_NOTATION_OBJECT_LIST_H
#define KINETIC_NOTATION_OBJECT_LIST_H

#include <stdbool.h>

struct object;

struct version {
  int major, minor, patch;
};

typedef union {
  char *string;
  struct version version;
  bool boolean;
} value_types;

struct statement {
  char *key;
  struct statement *next;
};

#define YYSTYPE value_types

struct object {
  struct statement *first_statement;
  struct object *next;
};

#endif // KINETIC_NOTATION_OBJECT_LIST_H
