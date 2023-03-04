#ifndef KINETIC_NOTATION_H
#define KINETIC_NOTATION_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct key {
  char *key;
  union {
    enum {
      STRING,
      NUMBER,
      VERSION,
      BOOLEAN,
      VARIABLE_KEY_ARRAY,
    } value_type;
    struct key *object_array_outline;
  };
};

#define OUTLINE(outline_name) struct key outline_name[]
#define OBJECT_ARRAY_OUTLINE (struct key[])

typedef struct Structure_t Structure;

typedef struct {
  int major;
  int minor;
  int patch;
} KineticVersion;

typedef struct {
  union {
    char *string;
    KineticVersion version;
    int number;
    bool boolean;
  };
  bool filled;
} KineticValue;

extern Structure *parse_file(char *file_path, struct key outline[]);

extern KineticValue structure_get_key(Structure *structure, char *key);
extern void structure_get_object_array(Structure *structure, char *key,
                                       int *array_length,
                                       Structure **object_array);

#endif // KINETIC_NOTATION_H
