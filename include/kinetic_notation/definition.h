#ifndef KINETIC_NOTATION_KN_DEFINITION_H
#define KINETIC_NOTATION_KN_DEFINITION_H

#include "types.h"

// std
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct kn_definition kn_definition;

/**
 * @brief This function creates a new kn_definition object, which is used to
 * define the structure of a Kinetic Notation object.
 *
 * @return A pointer to the newly created kn_definition object. If memory
 * allocation fails, this function returns NULL.
 */
kn_definition *kn_definition_new();

/**
 * @brief This function frees the memory allocated for a kn_definition object.
 *
 * @param definition: A pointer to the kn_definition object to be destroyed.
 */
void kn_definition_destroy(kn_definition *definition);

kn_definition *kn_definition_copy(kn_definition *definition);

/**
 * @brief This function adds a boolean value to the kn_definition object with
 * the specified key.
 *
 * @param definition: A pointer to the kn_definition object to which the boolean
 * value should be added.
 * @param key: A string representing the key for the boolean value.
 *
 * @return A boolean indicating whether the boolean value was successfully added
 * to the kn_definition object. If the key already exists in the kn_definition
 * object or memory allocation fails, this function returns false.
 */

bool kn_definition_add_boolean(kn_definition *definition, char *key);

/**
 * @brief This function adds a number value to the kn_definition object with the
 * specified key.
 *
 * @param definition: A pointer to the kn_definition object to which the number
 * value should be added.
 * @param key: A string representing the key for the number value.
 *
 * @return A boolean indicating whether the number value was successfully added
 * to the kn_definition object. If the key already exists in the kn_definition
 * object or memory allocation fails, this function returns false.
 */
bool kn_definition_add_number(kn_definition *definition, char *key);

/**
 * @brief This function adds a string value to the kn_definition object with the
 * specified key.
 *
 * @param definition: A pointer to the kn_definition object to which the string
 * value should be added.
 * @param key: A string representing the key for the string value.
 *
 * @return A boolean indicating whether the string value was successfully added
 * to the kn_definition object. If the key already exists in the kn_definition
 * object or memory allocation fails, this function returns false.
 */
bool kn_definition_add_string(kn_definition *definition, char *key);

/**
 * @brief This function adds a version value to the kn_definition object with
 * the specified key.
 *
 * @param definition: A pointer to the kn_definition object to which the
 * version value should be added.
 * @param key: A string representing the key for the version value.
 *
 * @return A boolean indicating whether the version value was successfully added
 * to the kn_definition object. If the key already exists in the kn_definition
 * object or memory allocation fails, this function returns false.
 */
bool kn_definition_add_version(kn_definition *definition, char *key);

/**
 * @brief This function adds an object definition to the kn_definition object at
 * the specified key.
 *
 * The object definition specifies the structure and data types of the object
 * that can be added to object at this key. By adding an object definition to a
 * kn_definition object, you are defining the schema for a particular type of
 * object that your program can work with.
 *
 * @param definition: A pointer to the kn_definition object to which the
 * value should be added.
 * @param key: A string representing the key for the object value.
 * @param object_definition: A pointer to the kn_definition object that defines
 * the structure of the object.
 *
 * @return A boolean indicating whether the object value was successfully
 */
bool kn_definition_add_object(kn_definition *definition, char *key,
                              kn_definition *object_definition);

/**
 * @brief This function adds an object definition to be used throughout an array
 * to the kn_definition object at the specified key.
 *
 * The object definition specifies the structure and data types of the object
 * that can be added to the array of objects at this key. By adding an object
 * definition to a kn_definition object, you are defining the schema for a
 * particular type of object that your program can work with.
 *
 * @param definition The kn_definition object to add the array of objects to.
 * @param key The key to use for the array of objects.
 * @param object_definition The object definition that the objects in the array
 * must conform to.
 *
 * @return true if the array of objects was successfully added, false otherwise.
 */
bool kn_definition_add_object_array(kn_definition *definition, char *key,
                                    kn_definition *object_definition);

bool kinetic_notation_parse(kn_definition *definition, const char *string);

typedef enum {
  SUCCESS,
  NOT_FILLED_IN,
  NOT_A_KEY,
  WRONG_TYPE,
  INDEX_OUT_OF_BOUNDS,
  NULL_REFERENCE,
} kn_query_result;

kn_query_result kn_definition_get_boolean(kn_definition *definition, char *key,
                                          bool *boolean);

kn_query_result kn_definition_get_number(kn_definition *definition, char *key,
                                         uint64_t *number);

/// Free the string when done with it
kn_query_result kn_definition_get_string(kn_definition *definition, char *key,
                                         char **string);

kn_query_result kn_definition_get_version(kn_definition *definition, char *key,
                                          kn_version *version);

/// Gets a reference *DO NOT FREE*
kn_query_result kn_definition_get_object(kn_definition *definition, char *key,
                                         kn_definition **object);

kn_query_result kn_definition_get_object_array_length(kn_definition *definition,
                                                      char *key,
                                                      size_t *length);

kn_query_result kn_definition_get_object_from_array_at_index(
    kn_definition *definition, char *key, size_t index, kn_definition **object);

#endif // KINETIC_NOTATION_KN_DEFINITION_H
