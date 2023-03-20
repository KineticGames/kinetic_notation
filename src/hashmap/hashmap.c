#include "hashmap.h"

#include "bucket.h"

// std
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

struct hashmap {
  size_t member_size;
  hash_fn *hashing_function;
  struct bucket **buckets;
  uint32_t bucket_count;
};

#define DEFAULT_HASHING_FUNCTION_MAX_INDEX 25
static int32_t hashmap_default_hashing_function(const char *key);

static void hashmap_destroy_buckets(hashmap *hashmap);

hashmap *hashmap_new(size_t member_size) {
  hashmap *hashmap = malloc(sizeof(struct hashmap));
  hashmap->member_size = member_size;
  hashmap->bucket_count = DEFAULT_HASHING_FUNCTION_MAX_INDEX + 1;
  hashmap->buckets =
      calloc(DEFAULT_HASHING_FUNCTION_MAX_INDEX + 1, sizeof(struct bucket));
  hashmap->hashing_function = hashmap_default_hashing_function;
  return hashmap;
}

void hashmap_destroy(hashmap *hashmap) {
  hashmap_destroy_buckets(hashmap);
  free(hashmap);
}

void hashmap_set_hashing_function(hashmap *hashmap, hash_fn *function,
                                  uint32_t max_index) {
  hashmap_destroy_buckets(hashmap);
  hashmap->bucket_count = max_index + 1;
  hashmap->buckets = malloc(sizeof(struct bucket) * max_index + 1);
  hashmap->hashing_function = function;
}

bool hashmap_insert(hashmap *hashmap, const char *key, const void *value) {
  if (key == NULL || value == NULL) {
    return false;
  }

  int32_t index = hashmap->hashing_function(key);
  if (index < 0) {
    return false;
  }

  struct bucket *new_bucket = bucket_create(key, value, hashmap->member_size);

  if (hashmap->buckets[index] == NULL) {
    hashmap->buckets[index] = new_bucket;
    return true;
  }

  struct bucket *b = hashmap->buckets[index];
  while (b->next != NULL) {
    b = b->next;
  }
  b->next = new_bucket;

  return true;
}

const void *hashmap_get(const hashmap *hashmap, const char *key) {
  int32_t index = hashmap->hashing_function(key);
  if (index < 0 || index > hashmap->bucket_count) {
    return NULL;
  }

  struct bucket *b = hashmap->buckets[index];
  while (b != NULL) {
    if (strcmp(b->key, key) == 0) {
      return b->value;
    }
    b = b->next;
  }
  return NULL;
}

void hashmap_remove(hashmap *hashmap, const char *key) {
  int32_t index = hashmap->hashing_function(key);
  if (index < 0 || index > hashmap->bucket_count) {
    return;
  }

  struct bucket *b = hashmap->buckets[index];
  struct bucket *p = NULL;
  while (b->next != NULL) {
    if (strcmp(b->key, key) == 0) {
      if (p == NULL && b->next != NULL) {
        memcpy(&hashmap->buckets[index], b->next, sizeof(struct bucket));
        free(b->next);
      } else {
        p->next = b->next;
        free(b);
      }
    }
    p = b;
    b = b->next;
  }
  return;
}

hashmap *hashmap_copy(hashmap *source) {
  hashmap *copy = hashmap_new(source->member_size);
  hashmap_set_hashing_function(copy, source->hashing_function,
                               source->bucket_count);

  for (size_t i = 0; i < source->bucket_count; ++i) {
    if (source->buckets[i] != NULL) {
      copy->buckets[i] = malloc(sizeof(struct bucket));
      bucket_copy(copy->buckets[i], source->buckets[i], source->member_size);
    }
  }

  return copy;
}

static int32_t hashmap_default_hashing_function(const char *key) {
  if (key == NULL || key[0] < 'a' || key[0] > 'z') {
    return -1;
  }
  return key[0] - 'a';
}

static void hashmap_destroy_buckets(hashmap *hashmap) {
  for (int i = 0; i < hashmap->bucket_count; ++i) {
    bucket_destroy(hashmap->buckets[i]);
  }
  free(hashmap->buckets);
}
