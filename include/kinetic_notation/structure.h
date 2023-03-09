#ifndef KINETIC_NOTATION_KINETIC_NOTATION_STRUCTURE_H
#define KINETIC_NOTATION_KINETIC_NOTATION_STRUCTURE_H

#include "result.h"
#include "types.h"

// std
#include <stddef.h>
#include <stdint.h>

struct KnKeyCreateInfo;

typedef struct KnStructureCreateInfo {
  uint32_t keyCount;
  struct KnKeyCreateInfo *keys;
} KnStructureCreateInfo;

typedef enum KnValueType {
  STRING,
  NUMBER,
  VERSION,
  BOOLEAN,
  SUB_OBJECT,
  OBJECT_ARRAY,
  VARIABLE_KEY_ARRAY,
} KnValueType;

typedef struct KnKeyCreateInfo {
  char *key;
  KnValueType type;
  union {
    struct KnStructureCreateInfo object_outline;
    KnValueType variable_key_array_type;
  } with;
} KnKeyCreateInfo;

typedef struct KnStructure_t *KnStructure;

KnResult
kinetic_notation_structure_create(const KnStructureCreateInfo *createInfo,
                                  KnStructure *structure);
void kinetic_notation_structure_destroy(KnStructure structure);

KnResult kinetic_notation_structure_parse(const char *buffer,
                                          KnStructure structure);

KnResult kinetic_notation_structure_get_key(KnStructure structure,
                                            const char *key, KnValue *value);
KnResult kinetic_notation_structure_get_object_array(KnStructure structure,
                                                     const char *key,
                                                     uint32_t *array_length,
                                                     KnStructure *object_array);

#endif // KINETIC_NOTATION_KINETIC_NOTATION_STRUCTURE_H
