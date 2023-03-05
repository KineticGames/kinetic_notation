#include "kinetic_notation.h"

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

KnResult enumerate_string_split_at(const char *string, char split_char,
                                   uint32_t *part_count, char **key_parts) {
  if (string == NULL) {
    return INVALID_ARGUMENT;
  }

  if (key_parts == NULL) {
    if (string[0] == split_char) {
      return INVALID_KEY;
    }

    uint32_t count = 0;

    for (int i = 1; i < strlen(string); ++i) {
      char c = string[i];

      if (c == split_char) {
        ++count;

        if (string[i + 1] == split_char) {
          return INVALID_KEY;
        }
      }
    }

    *part_count = count;
  } else {
    size_t total_len = strlen(string) + 1;

    int prev_part = 0;
    int p = 0;
    for (int i = 1; i < total_len; ++i) {
      if (p < *part_count) {
        return SUCCESS;
      }

      char c = string[i];
      if (c == split_char) {
        int len = i - prev_part;

        key_parts[p] = malloc(len);
        memcpy(key_parts[p], &string[prev_part + 1], len);
        key_parts[p][len - 1] = '\0';

        prev_part = i;
        p++;;
      }
    }
    int len = total_len - prev_part;
    key_parts[p] = malloc(len);
    memcpy(key_parts[p], &string[prev_part + 1], len);
  }

  return SUCCESS;
}

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

void knGetObjectArrayFromStructure(KnStructure structure, const char *key,
                                   uint32_t *array_length,
                                   KnStructure *object_array) {}
