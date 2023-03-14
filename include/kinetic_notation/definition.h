#ifndef KINETIC_NOTATION_KN_DEFINITION_H
#define KINETIC_NOTATION_KN_DEFINITION_H

#include "result.h"
#include "types.h"

// std
#include <stddef.h>
#include <stdint.h>

kn_definition *kn_definition_new();
void kn_definition_destroy(kn_definition *definition);
bool kn_definition_add_boolean(kn_definition *definition, char *key);
bool kn_definition_add_number(kn_definition *definition, char *key);
bool kn_definition_add_string(kn_definition *definition, char *key);
bool kn_definition_add_version(kn_definition *definition, char *key);
bool kn_definition_add_object(kn_definition *definition, char *key,
                              kn_definition *object_definition);
bool kn_definition_add_object_array(kn_definition *definition, char *key,
                                    kn_definition *object_definition);

#endif // KINETIC_NOTATION_KN_DEFINITION_H
