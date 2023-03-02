#ifndef KINETIC_NOTATION_TOKEN_INFO_H
#define KINETIC_NOTATION_TOKEN_INFO_H

#include <stdbool.h>
#include <stdio.h>

typedef struct {
  int line_start;
  int line_end;
  int column_start;
  int column_end;
  bool valid;
  bool empty;
  bool specified;
} token_info;

token_info token_info_create(int line_start, int line_end, int column_start,
                             int column_end);
token_info token_info_create_empty();
void token_info_combine(token_info *self, const token_info *other);
void token_info_print(FILE *file, const token_info *self);

#endif // KINETIC_NOTATION_TOKEN_INFO_H
