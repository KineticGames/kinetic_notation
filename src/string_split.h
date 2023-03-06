#ifndef KINETIC_NOTATION_STRING_SPLIT_H
#define KINETIC_NOTATION_STRING_SPLIT_H

#include "kinetic_notation.h"

// std
#include <stdint.h>

KnResult enumerate_string_split_at(const char *string, char split_char,
                                   uint32_t *part_count, char **key_parts);

#endif // KINETIC_NOTATION_STRING_SPLIT_H
