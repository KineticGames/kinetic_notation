#ifndef KINETIC_NOTATION_HASHMAP_H
#define KINETIC_NOTATION_HASHMAP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct hashmap hashmap;

/**
 * should return an index within the
 */
typedef int32_t(hash_fn)(const char *key);

hashmap *hashmap_new(size_t member_size);

void hashmap_destroy(hashmap *hashmap);

hashmap *hashmap_copy(hashmap *source);

/**
 * @param function: should return an index within the range [0,max_index] or -1
 * when the key supplied is invalid;
 */
void hashmap_set_hashing_function(hashmap *hashmap, hash_fn *function,
                                  uint32_t max_index);

bool hashmap_insert(hashmap *hashmap, const char *key, const void *value);

const void *hashmap_get(const hashmap *hashmap, const char *key);

void hashmap_remove(hashmap *hashmap, const char *key);

typedef struct hashmap_iter hashmap_iter;

hashmap_iter *hashmap_make_iter(hashmap *hashmap);

void hashmap_iter_destroy(hashmap_iter *iter);

void *hashmap_iter_value(hashmap_iter *iter);

void hashmap_iter_next(hashmap_iter *iter);

bool hashmap_iter_at_end(hashmap_iter *iter);

#endif // KINETIC_NOTATION_HASHMAP_H
