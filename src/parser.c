#include "parser.h"

#include "error.h"
#include "scanner.h"
#include "structure.h"

// std
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_BUF_MAX_SIZE 100

typedef struct Parser {
  Scanner scanner;
  Token current;
  Token previous;
} Parser;

static KnResult advance(Parser parser);
static bool check(Parser parser, TokenType type);
static KnResult consume(Parser parser, TokenType type, const char *message,
                        KnResult associated_result);
static KnResult error(Parser parser, const char *message,
                      KnResult associated_result);
static KnResult error_at(Token *token, const char *message,
                         KnResult associated_result);
static KnResult error_at_current(Parser parser, const char *message,
                                 KnResult associated_result);
static KnResult match(Parser parser, TokenType type);
static KnResult statement(KnStructure structure, Parser parser);

KnResult parse(KnStructure structure, const char *source) {
  KnResult result;
  Parser parser = {};
  parser.scanner = scanner_init(source);

  while ((result = match(parser, TOKEN_EOF)) == NO_MATCH) {
    if ((result = statement(structure, parser)) != SUCCESS) {
      return result;
    }
  }

  return result;
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

static bool check(Parser parser, TokenType type) {
  return parser.current.type == type;
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

static KnResult match(Parser parser, TokenType type) {
  if (!check(parser, type)) {
    return NO_MATCH;
  }
  return advance(parser);
}

static KnResult parse_key(KnStructure structure, Parser parser,
                          KVPair **kv_pair) {
  KnResult result;
  if ((result = consume(parser, TOKEN_IDENTIFIER, "Expect key name.",
                        PARSER_ERROR)) != SUCCESS)
    return result;

  if ((*kv_pair = structure_find_key(structure, parser.previous.start)) ==
      NULL) {
    return error(parser, "Key not defined in structure.", KEY_NOT_FOUND);
  }

  consume(parser, TOKEN_COLON, "Expect ':' after key definition.",
          PARSER_ERROR);

  return SUCCESS;
}

static KnResult parse_string(Parser parser, KVPair *kv_pair) {
  KnResult result =
      consume(parser, TOKEN_STRING, "Expect string declaration", PARSER_ERROR);
  if (result != SUCCESS) {
    return result;
  }
  kv_pair->value.string =
      strndup(parser.previous.start + 1, parser.previous.length - 2);
  kv_pair->filled = true;
  return SUCCESS;
}

static KnResult parse_number(Parser parser, KVPair *kv_pair) {
  KnResult result =
      consume(parser, TOKEN_NUMBER, "Expect number declaration", PARSER_ERROR);
  if (result != SUCCESS) {
    return result;
  }
  kv_pair->value.number = atoi(parser.previous.start);
  kv_pair->filled = true;
  return SUCCESS;
}

static KnResult parse_version(Parser parser, KVPair *kv_pair) {
  KnResult result =
      consume(parser, TOKEN_NUMBER, "Expect version declaration", PARSER_ERROR);
  if (result != SUCCESS) {
    return result;
  }
  KnVersion v;
  sscanf(parser.previous.start, "%d.%d.%d", &v.major, &v.minor, &v.patch);
  kv_pair->value.version = v;
  kv_pair->filled = true;
  return SUCCESS;
}

static KnResult parse_boolean(Parser parser, KVPair *kv_pair) {
  KnResult result;
  if ((result = match(parser, TOKEN_TRUE)) != NO_MATCH) {
    if (result != SUCCESS) {
      return result;
    }

    kv_pair->value.boolean = true;
    kv_pair->filled = true;
  } else if ((result = match(parser, TOKEN_FALSE)) != NO_MATCH) {
    if (result != SUCCESS) {
      return result;
    }

    kv_pair->value.boolean = false;
    kv_pair->filled = true;
  } else {
    result = advance(parser);
    if (result != SUCCESS) {
      return result;
    }

    return error_at_current(parser, "Expected one of 'true' or 'false'",
                            PARSER_ERROR);
  }
  consume(parser, TOKEN_STRING, "Expect newline after statement.",
          PARSER_ERROR);
  return SUCCESS;
}

static KnResult parse_sub_object(Parser parser, KVPair *kv_pair) {
  return SUCCESS;
}

static KnResult parse_object_array(Parser parser, KVPair *kv_pair) {
  return SUCCESS;
}

static KnResult parse_variable_key_array(Parser parser, KVPair *kv_pair) {
  return SUCCESS;
}

static KnResult statement(KnStructure structure, Parser parser) {
  KnResult result;
  KVPair *pair;

  if ((result = parse_key(structure, parser, &pair)) != SUCCESS) {
    return result;
  }

  switch (pair->type) {
  case STRING:
    result = parse_string(parser, pair);
    break;
  case NUMBER:
    result = parse_number(parser, pair);
    break;
  case VERSION:
    result = parse_version(parser, pair);
    break;
  case BOOLEAN:
    result = parse_boolean(parser, pair);
    break;
  case SUB_OBJECT:
    result = parse_sub_object(parser, pair);
    break;
  case OBJECT_ARRAY:
    result = parse_object_array(parser, pair);
    break;
  case VARIABLE_KEY_ARRAY:
    result = parse_variable_key_array(parser, pair);
    break;
  }

  return result;
}
