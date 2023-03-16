#include "bucket.h"

// std
#include <stdlib.h>
#include <string.h>

struct bucket bucket_create(const char *key, const void *value,
                            size_t value_size) {
  struct bucket bucket;
  bucket.key = strdup(key);
  bucket.value = malloc(value_size);
  memcpy(bucket.value, value, value_size);
  bucket.next = NULL;
  return bucket;
}

void bucket_destroy(struct bucket bucket) {
  free(bucket.key);
  free(bucket.value);
  free(bucket.next);
  if (bucket.next == NULL) {
    return;
  }
  bucket_destroy(*bucket.next);
}
