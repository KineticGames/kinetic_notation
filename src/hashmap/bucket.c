#include "bucket.h"

// std
#include <stdlib.h>
#include <string.h>

struct bucket *bucket_create(const char *key, const void *value,
                             size_t value_size) {
  struct bucket *bucket = malloc(sizeof(struct bucket));
  bucket->key = strdup(key);
  bucket->value = malloc(value_size);
  memcpy(bucket->value, value, value_size);
  bucket->next = NULL;
  return bucket;
}

void bucket_destroy(struct bucket *bucket) {
  if (bucket == NULL) {
    return;
  }

  free(bucket->key);
  free(bucket->value);
  if (bucket->next == NULL) {
    return;
  }
  bucket_destroy(bucket->next);
  free(bucket);
}

void bucket_copy(struct bucket *dest, const struct bucket *source,
                 size_t value_size) {
  if (source == NULL) {
    return;
  }
  dest->key = strdup(source->key);
  dest->value = malloc(value_size);
  memcpy(dest->value, source->value, value_size);

  if (source->next != NULL) {
    dest->next = malloc(sizeof(struct bucket));
  }

  bucket_copy(dest->next, source->next, value_size);
}
