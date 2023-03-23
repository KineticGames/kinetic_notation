#include "parser.h"

#include "definition.h"
#include "error.h"
#include "hashmap.h"
#include "kinetic_notation/definition.h"
#include "scanner.h"

// std
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_BUF_MAX_SIZE 200

typedef struct Parser_t {
  Scanner scanner;
  Token current;
  Token previous;
} *Parser;

static void error(Parser parser, const char *message);
static void error_at(Token token, const char *message);
static void error_at_current(Parser parser, const char *message);

static bool advance(Parser parser);
static bool check(Parser parser, TokenType type);
static bool consume(Parser parser, TokenType type, const char *message);
static bool match(Parser parser, TokenType type);

static struct value *parse_key(kn_definition *definition, Parser parser);
static bool parse_object(Parser parser, kn_definition *definition);

static bool parse_value_as_string(Parser parser, struct value *value);
static bool parse_value_as_number(Parser parser, struct value *value);
static bool parse_value_as_version(Parser parser, struct value *value);
static bool parse_value_as_boolean(Parser parser, struct value *value);
static bool parse_value_as_object(Parser parser, struct value *value);
static bool parse_value_as_object_array(Parser parser, struct value *value);

static bool statement(kn_definition *definition, Parser parser);

bool parse(kn_definition *structure, const char *string) {
  Parser parser = malloc(sizeof(struct Parser_t));
  parser->scanner = scanner_init(string);

  if (!advance(parser)) {
    return false;
  }

  while (!match(parser, TOKEN_EOF)) {
    if (!statement(structure, parser)) {
      error(parser, "Expect statement");
      return false;
    }
  }

  free(parser->scanner);
  free(parser);

  return true;
}

static bool advance(Parser parser) {
  parser->previous = parser->current;

  for (;;) {
    parser->current = scanner_scan_token(parser->scanner);

    if (parser->current.type != TOKEN_ERROR) {
      break;
    }

    error_at_current(parser, parser->current.start);
    return false;
  }

  return true;
}

static bool check(Parser parser, TokenType type) {
  return parser->current.type == type;
}

static bool consume(Parser parser, TokenType type, const char *message) {
  if (parser->current.type == type) {
    return advance(parser);
  }

  error_at_current(parser, message);
  return false;
}

static void error(Parser parser, const char *message) {
  error_at(parser->previous, message);
}

static void error_at(Token token, const char *message) {
  char error_buf[ERROR_BUF_MAX_SIZE];

  switch (token.type) {
  case TOKEN_EOF:
    snprintf(error_buf, ERROR_BUF_MAX_SIZE, "[line %d] Error at end: %s\n",
             token.line, message);
    break;
  case TOKEN_ERROR:
    snprintf(error_buf, ERROR_BUF_MAX_SIZE, "[line %d] Error: %s\n", token.line,
             message);
  default:
    snprintf(error_buf, ERROR_BUF_MAX_SIZE, "[line %d] Error at '%.*s': %s\n",
             token.line, token.length, token.start, message);
    break;
  }

  add_error(error_buf);
}

static void error_at_current(Parser parser, const char *message) {
  return error_at(parser->current, message);
}

static bool match(Parser parser, TokenType type) {
  if (!check(parser, type)) {
    return false;
  }
  return advance(parser);
}

static struct value *parse_key(kn_definition *definition, Parser parser) {
  if (!consume(parser, TOKEN_IDENTIFIER, "Expect key name.")) {
    return NULL;
  }

  char *key = strndup(parser->previous.start, parser->previous.length);
  struct value *value = (struct value *)hashmap_get(definition->keys, key);
  free(key);
  if (value == NULL) {
    error(parser, "Key not defined in structure.");
    return NULL;
  }

  if (!consume(parser, TOKEN_COLON, "Expect ':' after key definition.")) {
    return NULL;
  }

  return value;
}

static bool parse_object(Parser parser, kn_definition *definition) {
  if (!consume(parser, TOKEN_LEFT_BRACE, "Expect '{'.")) {
    return false;
  }

  if (!consume(parser, TOKEN_NEWLINE, "Expect newline after opening object.")) {
    return false;
  }

  while (!match(parser, TOKEN_RIGHT_BRACE)) {
    if (!statement(definition, parser)) {
      error(parser, "Expect statement");
      return false;
    }
  }

  return consume(parser, TOKEN_NEWLINE, "Expect newline after closing object.");
}

static bool parse_value_as_string(Parser parser, struct value *value) {
  if (!consume(parser, TOKEN_STRING, "Expect string declaration")) {
    return false;
  }
  value->as.string =
      strndup(parser->previous.start + 1, parser->previous.length - 2);
  value->is_specified = true;
  return consume(parser, TOKEN_NEWLINE, "Expect newline after statement.");
}

static bool parse_value_as_number(Parser parser, struct value *value) {
  if (!consume(parser, TOKEN_NUMBER, "Expect number declaration")) {
    return false;
  }

  value->as.number = strtoul(parser->previous.start, NULL, 10);
  value->is_specified = true;
  return consume(parser, TOKEN_NEWLINE, "Expect newline after statement.");
}

static bool parse_value_as_version(Parser parser, struct value *value) {
  if (!consume(parser, TOKEN_VERSION, "Expect version declaration")) {
    return false;
  }

  kn_version v;
  sscanf(parser->previous.start, "%ld.%ld.%ld", &v.major, &v.minor, &v.patch);

  value->as.version = v;
  value->is_specified = true;

  return consume(parser, TOKEN_NEWLINE, "Expect newline after statement.");
}

static bool parse_value_as_boolean(Parser parser, struct value *value) {
  if (match(parser, TOKEN_TRUE)) {
    value->as.boolean = true;
    value->is_specified = true;
  } else if (match(parser, TOKEN_FALSE)) {
    value->as.boolean = false;
    value->is_specified = true;
  } else {
    if (!advance(parser)) {
      return false;
    }

    error_at_current(parser, "Expect one of 'true' or 'false'");
    return false;
  }
  return consume(parser, TOKEN_NEWLINE, "Expect newline after statement.");
}

static bool parse_value_as_object(Parser parser, struct value *value) {
  if (parse_object(parser, value->as.object)) {
    value->is_specified = true;
    return true;
  }
  return false;
}

static bool parse_value_as_object_array(Parser parser, struct value *value) {
  if (!consume(parser, TOKEN_LEFT_BRACKET, "Expected '['.")) {
    return false;
  }

  if (!consume(parser, TOKEN_NEWLINE, "Expect newline after starting array.")) {
    return false;
  }

  struct object_array_node {
    kn_definition *definition;
    struct object_array_node *next;
  };

  size_t object_count = 0;
  struct object_array_node *object_array = NULL;
  while (!match(parser, TOKEN_RIGHT_BRACKET)) {
    struct object_array_node *node = malloc(sizeof(struct object_array_node));
    node->definition =
        kn_definition_copy(value->as.object_array.object_definition);

    if (!parse_object(parser, node->definition)) {
      return false;
    }

    node->next = NULL;

    if (object_array == NULL) {
      object_array = node;
    } else {
      struct object_array_node *n = object_array;
      while (n->next != NULL) {
        n = n->next;
      }
      n->next = node;
    }
    object_count++;
  }

  kn_definition *array[object_count];
  struct object_array_node *n = object_array;
  for (int i = 0; i < object_count; ++i) {
    array[i] = n->definition;
    n = n->next;
  }
  size_t array_size = object_count * sizeof(kn_definition *);
  value->as.object_array.array = malloc(array_size);
  memcpy(value->as.object_array.array, array, array_size);
  value->as.object_array.object_count = object_count;
  value->is_specified = true;

  n = object_array;
  while (n != NULL) {
    struct object_array_node *next = n->next;
    free(n);
    n = next;
  }

  return consume(parser, TOKEN_NEWLINE, "Expect newline after array.");
}

static bool statement(kn_definition *definition, Parser parser) {
  struct value *value = parse_key(definition, parser);
  if (value == NULL) {
    return false;
  }

  switch (value->type) {
  case STRING:
    return parse_value_as_string(parser, value);
  case NUMBER:
    return parse_value_as_number(parser, value);
  case VERSION:
    return parse_value_as_version(parser, value);
  case BOOLEAN:
    return parse_value_as_boolean(parser, value);
  case OBJECT:
    return parse_value_as_object(parser, value);
  case OBJECT_ARRAY:
    return parse_value_as_object_array(parser, value);
  }

  add_error("Internal Parsing Error");
  return false;
}
