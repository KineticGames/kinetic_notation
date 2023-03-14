#ifndef KINETIC_NOTATION_KEY_VALUE_PAIR_H
#define KINETIC_NOTATION_KEY_VALUE_PAIR_H

#include "kinetic_notation/definition.h"

typedef struct KVPair_t {
  char *key;
  KnValueType type;
  union {
    KnValue value;
    struct KnStructure_t *object;
    struct {
      KnStructureCreateInfo createInfo;
      uint32_t objectCount;
      KnStructure *array;
    } object_array;
    struct {
      KnValueType type;
      struct variable_key_array_list_node {
        struct KVPair_t *pair;
        struct variable_key_array_list_node *next;
      } *variable_key_array_list;
    } variable_key_array;
  };
  bool filled;
} KVPair;

#endif // KINETIC_NOTATION_KEY_VALUE_PAIR_H
