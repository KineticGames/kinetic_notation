#ifndef KINETIC_NOTATION_TYPES_H
#define KINETIC_NOTATION_TYPES_H

// std
#include <stdbool.h>

typedef struct {
  int major;
  int minor;
  int patch;
} KnVersion;

typedef union {
  char *string;
  KnVersion version;
  int number;
  bool boolean;
} KnValue;

#endif // KINETIC_NOTATION_TYPES_H
