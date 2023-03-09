#include "kinetic_notation.h"
#include "string_split.h"

// std
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct KnStructure_t {
  struct KVPair {
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
  } *keys;
  uint32_t key_count;
};

KnResult create_structure_from_create_info(KnStructureCreateInfo createInfo,
                                           struct KnStructure_t *structure);

KnResult
kinetic_notation_structure_create(const KnStructureCreateInfo *createInfo,
                                  KnStructure *structure) {
  if (createInfo == NULL || structure == NULL) {
    return INVALID_ARGUMENT;
  }

  struct KnStructure_t created_structure = {};
  KnResult result =
      create_structure_from_create_info(*createInfo, &created_structure);
  if (result != SUCCESS) {
    return result;
  }

  *structure = malloc(sizeof(struct KnStructure_t));
  *structure =
      memcpy(*structure, &created_structure, sizeof(struct KnStructure_t));

  return SUCCESS;
}

void kinetic_notation_structure_destroy(struct KnStructure_t *structure) {
  for (int i = 0; i < structure->key_count; ++i) {
    if (structure->keys[i].type == OBJECT) {
      kinetic_notation_structure_destroy(structure->keys[i].object);
    }
    free(structure->keys[i].key);
  }
  free(structure->keys);
  free(structure);
}

KnResult kinetic_notation_structure_parse(char *buffer, size_t buffer_size,
                                          KnStructure structure) {
  // TODO
  return SUCCESS;
}

KnResult kinetic_notation_structure_get_key(KnStructure structure,
                                            const char *key, KnValue *value) {
  for (int i = 0; i < structure->key_count; ++i) {
    if (strcmp(structure->keys[i].key, key) == 0) {
      if (!structure->keys[i].filled) {
        return NO_VALUE;
      }
      *value = structure->keys[i].value;
      return SUCCESS;
    }
  }

  return KEY_NOT_FOUND;
}

KnResult kinetic_notation_structure_get_object_array(
    KnStructure structure, const char *key, uint32_t *array_length,
    KnStructure *object_array) {
  for (int i = 0; i < structure->key_count; ++i) {
    if (strcmp(structure->keys[i].key, key) == 0) {
      if (!structure->keys[i].filled) {
        return NO_VALUE;
      }
      *array_length = structure->keys[i].object_array.objectCount;
      *object_array = structure->keys[i].object_array.array;
      return SUCCESS;
    }
  }

  return KEY_NOT_FOUND;
}

KnResult create_structure_from_create_info(KnStructureCreateInfo createInfo,
                                           struct KnStructure_t *structure) {
  if (structure == NULL) {
    return INVALID_ARGUMENT;
  }

  size_t key_count = createInfo.keyCount;
  struct KVPair keys[key_count];
  for (int i = 0; i < key_count; ++i) {
    if (createInfo.keys[i].key == NULL) {
      return NULL_STRING;
    }

    keys[i].key = strdup(createInfo.keys[i].key);
    keys[i].type = createInfo.keys[i].type;
    keys[i].filled = false;

    if (createInfo.keys[i].type == OBJECT) {
      keys[i].object = malloc(sizeof(struct KnStructure_t));

      KnResult result = create_structure_from_create_info(
          createInfo.keys[i].objectOutline, keys[i].object);

      if (result != SUCCESS) {
        return result;
      }

    } else if (createInfo.keys[i].type == OBJECT_ARRAY) {
      for (int i = 0; i < createInfo.keys[i].objectOutline.keyCount; ++i) {
        if (createInfo.keys[i].objectOutline.keys[i].type == OBJECT_ARRAY) {
          return NESTED_OBJECT_ARRAY;
        }
      }

      keys[i].object_array.createInfo = createInfo.keys[i].objectOutline;
    }
  }

  size_t array_size = key_count * sizeof(struct KVPair);
  structure->keys = malloc(array_size);
  memcpy(structure->keys, keys, array_size);
  return SUCCESS;
}
