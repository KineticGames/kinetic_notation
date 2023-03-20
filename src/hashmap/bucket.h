#ifndef KINETIC_HASHMAP_BUCKET_H
#define KINETIC_HASHMAP_BUCKET_H

// std
#include <stddef.h>

struct bucket {
  char *key;
  void *value;
  struct bucket *next;
};

struct bucket *bucket_create(const char *key, const void *value,
                             size_t value_size);
void bucket_destroy(struct bucket *bucket);
void bucket_copy(struct bucket *dest, const struct bucket *source,
                 size_t value_size);

#endif // KINETIC_HASHMAP_BUCKET_H
