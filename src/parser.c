#include "parser.h"

#include "error.h"
#include "scanner.h"

// std
#include <stdio.h>

#define ERROR_BUF_MAX_SIZE 100

typedef struct Parser {
  Scanner scanner;
  Token current;
  Token previous;
} Parser;

static KnResult advance(Parser parser);
static KnResult consume(Parser parser, TokenType type, const char *message,
                        KnResult associated_result);
static KnResult error(Parser parser, const char *message,
                      KnResult associated_result);
static KnResult error_at(Token *token, const char *message,
                         KnResult associated_result);
static KnResult error_at_current(Parser parser, const char *message,
                                 KnResult associated_result);

KnResult parse(KnStructure structure, const char *source) {
  Parser parser;
  parser.scanner = scanner_init(source);

  return SUCCESS;
}

static KnResult advance(Parser parser) {
  parser.previous = parser.current;

  for (;;) {
    parser.current = scanner_scan_token(parser.scanner);

    if (parser.current.type != TOKEN_ERROR) {
      break;
    }

    return error_at_current(parser, parser.current.start, SCANNER_ERROR);
  }

  return SUCCESS;
}

static KnResult consume(Parser parser, TokenType type, const char *message,
                        KnResult associated_result) {
  if (parser.current.type == type) {
    return advance(parser);
  }

  return error_at_current(parser, message, associated_result);
}

static KnResult error(Parser parser, const char *message,
                      KnResult associated_result) {
  return error_at(&parser.previous, message, associated_result);
}

static KnResult error_at(Token *token, const char *message,
                         KnResult associated_result) {
  char error_buf[ERROR_BUF_MAX_SIZE];

  switch (token->type) {
  case TOKEN_EOF:
    snprintf(error_buf, ERROR_BUF_MAX_SIZE, "[line %d] Error at end: %s\n",
             token->line, message);
    break;
  case TOKEN_ERROR:
    snprintf(error_buf, ERROR_BUF_MAX_SIZE, "[line %d] Error: %s\n",
             token->line, message);
  default:
    snprintf(error_buf, ERROR_BUF_MAX_SIZE, "[line %d] Error at '%.*s': %s\n",
             token->line, token->length, token->start, message);
    break;
  }

  add_error(error_buf, associated_result);
  return associated_result;
}

static KnResult error_at_current(Parser parser, const char *message,
                                 KnResult associated_result) {
  return error_at(&parser.current, message, associated_result);
}
