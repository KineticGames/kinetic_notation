#ifndef KINETIC_NOTATION_H
#define KINETIC_NOTATION_H

#include "key_type.h"

#include <stdlib.h>

typedef void Structure;

/* File Structure */
extern Structure *structure_create();

extern void structure_add_key(Structure *structure, const char *key_name,
                              key_type key_type);

/* Object Arrays */
extern void object_array_create(const char *key_name);
// extern void object_array_add_key(object_array_reference *object_array,
//                                            const char *key_name,
//                                            key_type key_type);
// object_array_reference *
// object_array_add_object_array(object_array_reference *object_array,
//                               const char *key_name);
// variable_key_array_reference *
// object_array_add_variable_key_array(object_array_reference *object_array,
//                                     const char *key_name);
//
///* Variable Key Arrays */
// variable_key_array_reference *
// variable_key_array_reference_create(const char *key_name);

#endif // KINETIC_NOTATION_H
