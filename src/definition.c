#include "kinetic_notation.h"

#include "hashmap.h"

// std
#include <stdlib.h>

enum value_type {
  BOOLEAN,
  NUMBER,
  STRING,
  VERSION,
  OBJECT,
  OBJECT_ARRAY,
};

struct value {
  enum value_type type;
  kn_definition *object_definition;
};

struct value *create_value(enum value_type type,
                           kn_definition *object_definition) {
  struct value *value = malloc(sizeof(struct value));
  value->type = type;
  value->object_definition = object_definition;
  return value;
}

struct kn_definition {
  hashmap *keys;
};

kn_definition *kn_definition_new() {
  kn_definition *definition = malloc(sizeof(kn_definition));
  definition->keys = hashmap_new(sizeof(struct value));
  return definition;
}

void kn_definition_destroy(kn_definition *definition) {
  hashmap_destroy(definition->keys);
  free(definition);
}

bool kn_definition_add_boolean(kn_definition *definition, char *key) {
  return hashmap_insert(definition->keys, key, create_value(BOOLEAN, NULL));
}

bool kn_definition_add_number(kn_definition *definition, char *key) {
  return hashmap_insert(definition->keys, key, create_value(NUMBER, NULL));
}

bool kn_definition_add_string(kn_definition *definition, char *key) {
  return hashmap_insert(definition->keys, key, create_value(STRING, NULL));
}

bool kn_definition_add_version(kn_definition *definition, char *key) {
  return hashmap_insert(definition->keys, key, create_value(VERSION, NULL));
}

bool kn_definition_add_object(kn_definition *definition, char *key,
                              kn_definition *object_definition) {
  return hashmap_insert(definition->keys, key,
                        create_value(OBJECT, object_definition));
}

bool kn_definition_add_object_array(kn_definition *definition, char *key,
                                    kn_definition *object_definition) {
  return hashmap_insert(definition->keys, key,
                        create_value(OBJECT_ARRAY, object_definition));
}
