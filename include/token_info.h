#ifndef KINETIC_NOTATION_TOKEN_INFO_H
#define KINETIC_NOTATION_TOKEN_INFO_H

#include <stdbool.h>

typedef enum {
    INT,
    STRING,
    FLOAT,
    BOOL,
} token_type;

typedef struct {
    int line_start;
    int line_end;
    int column_start;
    int column_end;
    bool valid;
    bool empty;
    bool specified;
    token_type type;
    union {
	int int_value;
	char *string_value;
	float float_value;
	bool bool_value;
    } data;
} token_info;

token_info token_info_create(int line_start, int line_end, int column_start, int column_end);
token_info token_info_create_empty();
void token_info_combine(const token_info *other);
void token_info_print();

#endif // KINETIC_NOTATION_TOKEN_INFO_H

