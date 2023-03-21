#include "hashmap.h"
#include "kinetic_notation.h"

#include "definition.h"
#include "kinetic_notation/definition.h"
#include "parser.h"

// std
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct value create_value(enum value_type type);
static struct value create_object_value(kn_definition *object);
static struct value create_object_array_value(kn_definition *object_definition);

kn_definition *kn_definition_new() {
  kn_definition *definition = malloc(sizeof(kn_definition));
  definition->keys = hashmap_new(sizeof(struct value));
  return definition;
}

void kn_definition_destroy(kn_definition *definition) {
  hashmap_iter *iter = hashmap_make_iter(definition->keys);
  for (; !hashmap_iter_at_end(iter); hashmap_iter_next(iter)) {
    struct value *value = hashmap_iter_value(iter);
    if (value->type == STRING) {
      free(value->as.string);
    } else if (value->type == OBJECT) {
      kn_definition_destroy(value->as.object);
    } else if (value->type == OBJECT_ARRAY) {
      for (size_t i = 0; i < value->as.object_array.object_count; ++i) {
        kn_definition_destroy(value->as.object_array.array[i]);
      }
      free(value->as.object_array.array);
      kn_definition_destroy(value->as.object_array.object_definition);
    }
  }
  hashmap_iter_destroy(iter);
  hashmap_destroy(definition->keys);
  free(definition);
}

kn_definition *kn_definition_copy(kn_definition *source) {
  kn_definition *copy = malloc(sizeof(kn_definition));
  copy->keys = hashmap_copy(source->keys);
  return copy;
}

bool kn_definition_add_boolean(kn_definition *definition, char *key) {
  struct value value = create_value(BOOLEAN);
  return hashmap_insert(definition->keys, key, &value);
}

bool kn_definition_add_number(kn_definition *definition, char *key) {
  struct value value = create_value(NUMBER);
  return hashmap_insert(definition->keys, key, &value);
}

bool kn_definition_add_string(kn_definition *definition, char *key) {
  struct value value = create_value(STRING);
  return hashmap_insert(definition->keys, key, &value);
}

bool kn_definition_add_version(kn_definition *definition, char *key) {
  struct value value = create_value(VERSION);
  return hashmap_insert(definition->keys, key, &value);
}

bool kn_definition_add_object(kn_definition *definition, char *key,
                              kn_definition *object_definition) {
  struct value value = create_object_value(object_definition);
  return hashmap_insert(definition->keys, key, &value);
}

bool kn_definition_add_object_array(kn_definition *definition, char *key,
                                    kn_definition *object_definition) {
  struct value value = create_object_array_value(object_definition);
  return hashmap_insert(definition->keys, key, &value);
}

bool kinetic_notation_parse(kn_definition *definition, const char *string) {
  return parse(definition, string);
}

struct get_boolean_result kn_definition_get_boolean(kn_definition *definition,
                                                    char *key) {
  struct value *value = (struct value *)hashmap_get(definition->keys, key);
  if (value == NULL) {
    return (struct get_boolean_result){.result = NOT_A_KEY};
  }
  if (value->type != BOOLEAN) {
    return (struct get_boolean_result){.result = WRONG_TYPE};
  }
  if (value->is_specified == false) {
    return (struct get_boolean_result){.result = NOT_FILLED_IN};
  }
  return (struct get_boolean_result){.boolean = value->as.boolean,
                                     .result = SUCCESS};
}

struct get_number_result kn_definition_get_number(kn_definition *definition,
                                                  char *key) {
  struct value *value = (struct value *)hashmap_get(definition->keys, key);
  if (value == NULL) {
    return (struct get_number_result){.result = NOT_A_KEY};
  }
  if (value->type != NUMBER) {
    return (struct get_number_result){.result = WRONG_TYPE};
  }
  if (value->is_specified == false) {
    return (struct get_number_result){.result = NOT_FILLED_IN};
  }
  return (struct get_number_result){.number = value->as.boolean,
                                    .result = SUCCESS};
}

struct get_string_result kn_definition_get_string(kn_definition *definition,
                                                  char *key) {
  struct value *value = (struct value *)hashmap_get(definition->keys, key);
  if (value == NULL) {
    return (struct get_string_result){.result = NOT_A_KEY};
  }
  if (value->type != STRING) {
    return (struct get_string_result){.result = WRONG_TYPE};
  }
  if (value->is_specified == false) {
    return (struct get_string_result){.result = NOT_FILLED_IN};
  }
  return (struct get_string_result){.string = strdup(value->as.string),
                                    .result = SUCCESS};
}

struct get_version_result kn_definition_get_version(kn_definition *definition,
                                                    char *key) {
  struct value *value = (struct value *)hashmap_get(definition->keys, key);
  if (value == NULL) {
    return (struct get_version_result){.result = NOT_A_KEY};
  }
  if (value->type != VERSION) {
    return (struct get_version_result){.result = WRONG_TYPE};
  }
  if (value->is_specified == false) {
    return (struct get_version_result){.result = NOT_FILLED_IN};
  }
  return (struct get_version_result){.version = value->as.version,
                                     .result = SUCCESS};
}

struct get_object_result kn_definition_get_object(kn_definition *definition,
                                                  char *key) {
  struct value *value = (struct value *)hashmap_get(definition->keys, key);
  if (value == NULL) {
    return (struct get_object_result){.result = NOT_A_KEY};
  }
  if (value->type != OBJECT) {
    return (struct get_object_result){.result = WRONG_TYPE};
  }
  if (value->is_specified == false) {
    return (struct get_object_result){.result = NOT_FILLED_IN};
  }
  return (struct get_object_result){.object = value->as.object,
                                    .result = SUCCESS};
}

struct get_object_array_length_result
kn_definition_get_object_array_length(kn_definition *definition, char *key) {

  struct value *value = (struct value *)hashmap_get(definition->keys, key);
  if (value == NULL) {
    return (struct get_object_array_length_result){.result = NOT_A_KEY};
  }
  if (value->type != OBJECT_ARRAY) {
    return (struct get_object_array_length_result){.result = WRONG_TYPE};
  }
  if (value->is_specified == false) {
    return (struct get_object_array_length_result){.result = NOT_FILLED_IN};
  }
  return (struct get_object_array_length_result){
      .result = SUCCESS, .length = value->as.object_array.object_count};
}

struct get_object_at_index_result
kn_definition_get_object_from_array_at_index(kn_definition *definition,
                                             char *key, size_t index) {
  struct value *value = (struct value *)hashmap_get(definition->keys, key);
  if (value == NULL) {
    return (struct get_object_at_index_result){.result = NOT_A_KEY};
  }
  if (value->type != OBJECT_ARRAY) {
    return (struct get_object_at_index_result){.result = WRONG_TYPE};
  }
  if (value->is_specified == false) {
    return (struct get_object_at_index_result){.result = NOT_FILLED_IN};
  }
  if (index < 0 || index >= value->as.object_array.object_count) {
    return (struct get_object_at_index_result){.result = INDEX_OUT_OF_BOUNDS};
  }

  return (struct get_object_at_index_result){
      .object = value->as.object_array.array[index], .result = SUCCESS};
}

static struct value create_value(enum value_type type) {
  struct value value = {};
  value.type = type;
  value.is_specified = false;
  return value;
}

static struct value create_object_value(kn_definition *object) {
  struct value value = {};
  value.type = OBJECT;
  value.is_specified = false;
  value.as.object = object;
  return value;
}

static struct value
create_object_array_value(kn_definition *object_definition) {
  struct value value = {};
  value.type = OBJECT_ARRAY;
  value.is_specified = false;
  value.as.object_array.object_definition = object_definition;
  return value;
}
