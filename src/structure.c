#include "structure.h"

#include "kinetic_notation.h"
#include "parser.h"
#include "string_split.h"

// std
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct KnStructure_t {
  KVPair *keys;
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
    if (structure->keys[i].type == SUB_OBJECT) {
      kinetic_notation_structure_destroy(structure->keys[i].object);
    }
    free(structure->keys[i].key);
  }
  free(structure->keys);
  free(structure);
}

KnResult kinetic_notation_structure_parse(const char *buffer,
                                          KnStructure structure) {
  return parse(structure, buffer);
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
  KVPair keys[key_count];
  for (int i = 0; i < key_count; ++i) {
    if (createInfo.keys[i].key == NULL) {
      return NULL_STRING;
    }

    keys[i].key = strdup(createInfo.keys[i].key);
    keys[i].type = createInfo.keys[i].type;
    keys[i].filled = false;

    if (createInfo.keys[i].type == SUB_OBJECT) {
      keys[i].object = malloc(sizeof(struct KnStructure_t));

      KnResult result = create_structure_from_create_info(
          createInfo.keys[i].with.object_outline, keys[i].object);

      if (result != SUCCESS) {
        return result;
      }
    } else if (createInfo.keys[i].type == OBJECT_ARRAY) {
      for (int i = 0; i < createInfo.keys[i].with.object_outline.keyCount;
           ++i) {
        if (createInfo.keys[i].with.object_outline.keys[i].type ==
            OBJECT_ARRAY) {
          return NESTED_OBJECT_ARRAY;
        }
      }

      keys[i].object_array.createInfo = createInfo.keys[i].with.object_outline;
    } else if (createInfo.keys[i].type == VARIABLE_KEY_ARRAY) {
      keys[i].variable_key_array.type =
          createInfo.keys[i].with.variable_key_array_type;
    }
  }

  size_t array_size = key_count * sizeof(KVPair);
  structure->keys = malloc(array_size);
  memcpy(structure->keys, keys, array_size);
  structure->key_count = key_count;
  return SUCCESS;
}

KVPair *structure_find_key(KnStructure structure, const char *key) {
  for (int i = 0; i < structure->key_count; ++i) {
    if (strcmp(structure->keys[i].key, key) == 0) {
      return &structure->keys[i];
    }
  }

  return NULL;
}
