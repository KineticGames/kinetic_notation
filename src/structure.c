#include "kinetic_notation.h"
#include "string_split.h"

// std
#include <stdio.h>
#include <string.h>

struct KnStructure {
  struct KVPair {
    char *key;
    KnValueType type;
    union {
      KnValue value;
      struct KnStructure *object;
      struct {
        KnStructureCreateInfo createInfo;
        uint32_t objectCount;
        struct KnStructure *array;
      } object_array;
    };
    bool filled;
  } *keys;
  uint32_t key_count;
};

KnResult create_structure_from_create_info(KnStructureCreateInfo createInfo,
                                           struct KnStructure *structure) {
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
      keys[i].object = malloc(sizeof(struct KnStructure));

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

KnResult knCreateStructure(const KnStructureCreateInfo *createInfo,
                           KnStructure *structure) {
  if (createInfo == NULL || structure == NULL) {
    return INVALID_ARGUMENT;
  }

  struct KnStructure created_structure = {};
  KnResult result =
      create_structure_from_create_info(*createInfo, &created_structure);
  if (result != SUCCESS) {
    return result;
  }

  *structure = malloc(sizeof(struct KnStructure));
  *structure =
      memcpy(*structure, &created_structure, sizeof(struct KnStructure));

  return SUCCESS;
}

void knDestroyStructure(struct KnStructure *structure) {
  for (int i = 0; i < structure->key_count; ++i) {
    if (structure->keys[i].type == OBJECT) {
      knDestroyStructure(structure->keys[i].object);
    }
    free(structure->keys[i].key);
  }
  free(structure->keys);
  free(structure);
}

KnResult knParseUsingStructure(char *buffer, KnStructure structure);

KnResult knGetKeyFromStructure(KnStructure structure, const char *key,
                               KnValue *value) {
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

KnResult knGetObjectArrayFromStructure(KnStructure structure, const char *key,
                                       uint32_t *array_length,
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
