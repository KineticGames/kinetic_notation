#include "ir_token_info.h"

#include <stdint.h>

token_info token_info_create(int line_start, int line_end, int column_start,
                             int column_end) {
  return (token_info){
      .line_start = line_start,
      .line_end = line_end,
      .column_start = column_start,
      .column_end = column_end,
      .valid = true,
      .specified = true,
      .empty = false,
  };
}

token_info token_info_create_empty() {
  return (token_info){
      .line_start = INT32_MAX,
      .line_end = -1,
      .column_start = INT32_MAX,
      .column_end = -1,
      .valid = true,
      .specified = true,
      .empty = false,
  };
}

void token_info_combine(token_info *self, const token_info *other) {
  self->valid = self->valid && other->valid;

  if (other->line_start < self->line_start) {
    self->line_start = other->line_start;
    self->column_start = other->column_start;
  } else if (other->line_start == self->line_start) {
    if (other->column_start < self->column_start) {
      self->column_start = other->column_start;
    }
  }

  if (other->line_end > self->line_end) {
    self->line_end = other->line_end;
    self->column_end = other->column_end;
  } else if (other->line_end == self->line_end) {
    if (other->column_end > self->column_end) {
      self->column_end = other->column_end;
    }
  }
}

void token_info_print(FILE *file, const token_info *self) {
  fprintf(file, "%d (L%d) - %d (L%d)\n", self->column_start, self->line_start,
          self->column_end, self->line_end);
}
