#ifndef KINETIC_NOTATION_KEY_VALUE_PAIR_H
#define KINETIC_NOTATION_KEY_VALUE_PAIR_H

#include "kinetic_notation/structure.h"

typedef struct {
  char *key;
  KnValueType type;
  union {
    KnValue value;
    struct KnStructure_t *object;
    struct {
      KnStructureCreateInfo createInfo;
      uint32_t objectCount;
      struct KnStructure_t *array;
    } object_array;
  };
  bool filled;
} KVPair;

#endif // KINETIC_NOTATION_KEY_VALUE_PAIR_H
