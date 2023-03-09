#ifndef KINETIC_NOTATION_RESULT_H
#define KINETIC_NOTATION_RESULT_H

typedef enum KnResult {
  SUCCESS,
  INVALID_ARGUMENT,
  NESTED_OBJECT_ARRAY,
  NULL_STRING,
  NO_VALUE,
  INVALID_KEY,
  KEY_NOT_FOUND,
  PARSER_ERROR,
  SCANNER_ERROR,
} KnResult;

#endif // KINETIC_NOTATION_RESULT_H
