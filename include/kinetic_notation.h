#ifndef KINETIC_NOTATION_H
#define KINETIC_NOTATION_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum KnResult {
  SUCCESS,
  INVALID_ARGUMENT,
  NESTED_OBJECT_ARRAY,
  NULL_STRING,
  NO_VALUE,
  INVALID_KEY,
  KEY_NOT_FOUND,
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

typedef struct KnStructure *KnStructure;

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

KnResult knCreateStructure(const KnStructureCreateInfo *createInfo,
                           KnStructure *structure);
void knDestroyStructure(KnStructure structure);

KnResult knParseUsingStructure(char *buffer, size_t buffer_size,
                               KnStructure structure);

KnResult knGetKeyFromStructure(KnStructure structure, const char *key,
                               KnValue *value);
KnResult knGetObjectArrayFromStructure(KnStructure structure, const char *key,
                                       uint32_t *array_length,
                                       KnStructure *object_array);

#endif // KINETIC_NOTATION_H
