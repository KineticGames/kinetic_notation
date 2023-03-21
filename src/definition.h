#ifndef KINETIC_NOTATION_DEFINITION_H
#define KINETIC_NOTATION_DEFINITION_H

#include "hashmap.h"
#include "kinetic_notation/definition.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum value_type {
  BOOLEAN,
  NUMBER,
  STRING,
  VERSION,
  OBJECT,
  OBJECT_ARRAY,
};

struct value {
  enum value_type type;
  union {
    bool boolean;
    uint64_t number;
    kn_version version;
    char *string;
    kn_definition *object;
    struct {
      kn_definition *object_definition;
      size_t object_count;
      kn_definition **array;
    } object_array;
  } as;
  bool is_specified;
};

struct kn_definition {
  hashmap *keys;
};

#endif // KINETIC_NOTATION_DEFINITION_H
