#include "hashmap.h"
#include "kinetic_notation.h"

#include "definition.h"
#include "kinetic_notation/definition.h"
#include "parser.h"

// std
#include <stdlib.h>

static struct value create_value(enum value_type type);
static struct value create_object_value(kn_definition *object);
static struct value create_object_array_value(kn_definition *object_definition);

kn_definition *kn_definition_new() {
  kn_definition *definition = malloc(sizeof(kn_definition));
  definition->keys = hashmap_new(sizeof(struct value));
  return definition;
}

void kn_definition_destroy(kn_definition *definition) {
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
