#ifndef KINETIC_NOTATION_HASHMAP_H
#define KINETIC_NOTATION_HASHMAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct hashmap hashmap;

typedef struct iter iter;

/**
 * should return an index within the
 */
typedef int32_t(hash_fn)(const char *key);

hashmap *hashmap_new(size_t member_size);

void hashmap_destroy(hashmap *hashmap);

/**
 * @param function: should return an index within the range [0,max_index] or -1
 * when the key supplied is invalid;
 */
void hashmap_set_hashing_function(hashmap *hashmap, hash_fn *function,
                                  uint32_t max_index);

bool hashmap_insert(hashmap *hashmap, const char *key, const void *value);

const void *hashmap_get(const hashmap *hashmap, const char *key);

void hashmap_remove(hashmap *hashmap, const char *key);

iter *hashmap_iter(const hashmap *hashmap);

#endif // KINETIC_NOTATION_HASHMAP_H
