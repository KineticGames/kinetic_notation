#ifndef KINETIC_NOTATION_H
#define KINETIC_NOTATION_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum KnResult {
  SUCCESS,
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
  OBJECT_ARRAY,
} KnValueType;

typedef struct KnKeyCreateInfo {
  char *key;
  KnValueType type;
  struct KnStructureCreateInfo objectArrayOutline;
} KnKeyCreateInfo;

typedef struct KnStructure *KnStructure;

typedef struct {
  int major;
  int minor;
  int patch;
} KnVersion;

typedef struct {
  union value {
    char *string;
    KnVersion version;
    int number;
    bool boolean;
  } value;
  bool filled;
} KnValue;

KnResult knCreateStructure(const KnStructureCreateInfo *createInfo,
                           KnStructure *structure);
void knDestroyStructure(KnStructure structure);

KnResult knParseFileUsingStructure(char *file_path, KnStructure structure);

void knGetKeyFromStructure(KnStructure structure, KnValue *value);
void knGetObjectArrayFromStructure(KnStructure structure, char *key,
                                   uint32_t *array_length,
                                   KnStructure *object_array);

#endif // KINETIC_NOTATION_H
