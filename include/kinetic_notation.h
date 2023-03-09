#ifndef KINETIC_NOTATION_H
#define KINETIC_NOTATION_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum KnResult {
  SUCCESS,
  INVALID_ARGUMENT,
  NESTED_OBJECT_ARRAY,
  NULL_STRING,
  NO_VALUE,
  INVALID_KEY,
  KEY_NOT_FOUND,
  PARSER_ERROR,
  SCANNER_ERROR,
} KnResult;

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
} KnValueType;

typedef struct KnKeyCreateInfo {
  char *key;
  KnValueType type;
  struct KnStructureCreateInfo objectOutline;
} KnKeyCreateInfo;

typedef struct KnStructure_t *KnStructure;

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

char *kinetic_notation_get_error();

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

#endif // KINETIC_NOTATION_H
