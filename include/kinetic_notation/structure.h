#ifndef KINETIC_NOTATION_STRUCTURE_H
#define KINETIC_NOTATION_STRUCTURE_H

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
  OBJECT,
  OBJECT_ARRAY,
  VARIABLE_KEY_ARRAY,
} KnValueType;

typedef struct KnKeyCreateInfo {
  char *key;
  KnValueType type;
  struct KnStructureCreateInfo objectOutline;
} KnKeyCreateInfo;

typedef struct KnStructure_t *KnStructure;

KnResult
kinetic_notation_structure_create(const KnStructureCreateInfo *createInfo,
                                  KnStructure *structure);
void kinetic_notation_structure_destroy(KnStructure structure);

KnResult kinetic_notation_structure_parse(char *buffer, size_t buffer_size,
                                          KnStructure structure);

KnResult kinetic_notation_structure_get_key(KnStructure structure,
                                            const char *key, KnValue *value);
KnResult kinetic_notation_structure_get_object_array(KnStructure structure,
                                                     const char *key,
                                                     uint32_t *array_length,
                                                     KnStructure *object_array);

#endif // KINETIC_NOTATION_STRUCTURE_H
