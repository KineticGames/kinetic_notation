#include "string_split.h"

// std
#include <stdlib.h>
#include <string.h>

KnResult enumerate_string_split_at(const char *string, char split_char,
                                   uint32_t *part_count, char **key_parts) {
  if (string == NULL) {
    return INVALID_ARGUMENT;
  }

  if (key_parts == NULL) {
    if (string[0] == split_char) {
      return INVALID_KEY;
    }

    uint32_t count = 0;

    for (int i = 1; i < strlen(string); ++i) {
      char c = string[i];

      if (c == split_char) {
        ++count;

        if (string[i + 1] == split_char) {
          return INVALID_KEY;
        }
      }
    }

    *part_count = count;
  } else {
    size_t total_len = strlen(string) + 1;

    int prev_part = 0;
    int p = 0;
    for (int i = 1; i < total_len; ++i) {
      if (p < *part_count) {
        return SUCCESS;
      }

      char c = string[i];
      if (c == split_char) {
        int len = i - prev_part;

        key_parts[p] = malloc(len);
        memcpy(key_parts[p], &string[prev_part + 1], len);
        key_parts[p][len - 1] = '\0';

        prev_part = i;
        p++;
      }
    }
    int len = total_len - prev_part;
    key_parts[p] = malloc(len);
    memcpy(key_parts[p], &string[prev_part + 1], len);
  }

  return SUCCESS;
}
