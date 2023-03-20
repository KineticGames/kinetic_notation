#include "scanner.h"

// std
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct Scanner {
  const char *start;
  const char *current;
  int line;
};

static char advance(Scanner scanner);
static Token error_token(Scanner scanner, const char *message);
static Token make_token(Scanner scanner, TokenType type);
static Token identifier(Scanner scanner);
static TokenType identifier_type(Scanner scanner);
static bool is_alpha(char c);
static bool is_at_end(Scanner scanner);
static bool is_digit(char c);
static Token number_or_version(Scanner scanner);
static char peek(Scanner scanner);
static char peek_next(Scanner scanner);
static void skip_whitespace(Scanner scanner);
static Token string(Scanner scanner);

Scanner scanner_init(const char *source) {
  struct Scanner *scanner = malloc(sizeof(struct Scanner));

  scanner->start = source;
  scanner->current = source;
  scanner->line = 1;

  return scanner;
}

Token scanner_scan_token(Scanner scanner) {
  skip_whitespace(scanner);
  scanner->start = scanner->current;

  if (is_at_end(scanner)) {
    return make_token(scanner, TOKEN_EOF);
  }

  char c = advance(scanner);
  if (is_alpha(c)) {
    return identifier(scanner);
  }
  if (is_digit(c)) {
    return number_or_version(scanner);
  }

  if (c == '\n') {
    scanner->line++;

    // Consume consecutive newlines
    while (peek(scanner) == '\n') {
      advance(scanner);
      scanner->line++;
    }

    return make_token(scanner, TOKEN_NEWLINE);
  }

  switch (c) {
  case ('['):
    return make_token(scanner, TOKEN_LEFT_BRACKET);
  case (']'):
    return make_token(scanner, TOKEN_RIGHT_BRACKET);
  case ('{'):
    return make_token(scanner, TOKEN_LEFT_BRACE);
  case ('}'):
    return make_token(scanner, TOKEN_RIGHT_BRACE);
  case (':'):
    return make_token(scanner, TOKEN_COLON);
  case '"':
    return string(scanner);
  }

  return error_token(scanner, "Unexpected Character");
}

static char advance(Scanner scanner) {
  scanner->current++;
  return scanner->current[-1];
}

static TokenType check_keyword(Scanner scanner, int start, int length,
                               const char *rest, TokenType type) {
  if (scanner->current - scanner->start == start + length &&
      memcmp(scanner->start + start, rest, length) == 0) {
    return type;
  }
  return TOKEN_IDENTIFIER;
}

static Token error_token(Scanner scanner, const char *message) {
  return (Token){
      .type = TOKEN_ERROR,
      .start = message,
      .length = (int)(scanner->current - scanner->start),
      .line = scanner->line,
  };
}

static Token make_token(Scanner scanner, TokenType type) {
  return (Token){
      .type = type,
      .start = scanner->start,
      .length = (int)(scanner->current - scanner->start),
      .line = scanner->line,
  };
}

static Token number_or_version(Scanner scanner) {
  while (is_digit(peek(scanner)))
    advance(scanner);

  // Look if it is a version number instead.
  if (peek(scanner) == '.' && is_digit(peek_next(scanner))) {
    // Consume the ".".
    advance(scanner);

    while (is_digit(peek(scanner))) {
      advance(scanner);
    }

    if (peek(scanner) == '.' && is_digit(peek_next(scanner))) {
      // Consume the ".".
      advance(scanner);

      while (is_digit(peek(scanner))) {
        advance(scanner);
      }
    } else {
      return error_token(scanner, "Incomplete version.");
    }

    return make_token(scanner, TOKEN_VERSION);
  }

  return make_token(scanner, TOKEN_NUMBER);
}

static Token identifier(Scanner scanner) {
  while (is_alpha(peek(scanner)) || is_digit(peek(scanner))) {
    advance(scanner);
  }

  return make_token(scanner, identifier_type(scanner));
}

static TokenType identifier_type(Scanner scanner) {
  switch (scanner->start[0]) {
  case 'f':
    return check_keyword(scanner, 1, 4, "alse", TOKEN_FALSE);
  case 't':
    return check_keyword(scanner, 1, 3, "rue", TOKEN_TRUE);
  }

  return TOKEN_IDENTIFIER;
}

static bool is_alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool is_at_end(Scanner scanner) { return *scanner->current == '\0'; }

static bool is_digit(char c) { return c >= '0' && c <= '9'; }

static char peek(Scanner scanner) { return *scanner->current; }

static char peek_next(Scanner scanner) {
  if (is_at_end(scanner)) {
    return '\0';
  }
  return scanner->current[1];
}

static void skip_whitespace(Scanner scanner) {
  for (;;) {
    char c = peek(scanner);
    switch (c) {
    case ' ':
    case '\r':
    case '\t':
      advance(scanner);
      break;
    case '#':
      while (peek(scanner) != '\n' && !is_at_end(scanner)) {
        advance(scanner);
      }
      break;
    default:
      return;
    }
  }
}

static Token string(Scanner scanner) {
  while (peek(scanner) != '"' && !is_at_end(scanner)) {
    if (peek(scanner) == '\n') {
      return error_token(scanner, "Multiline string.");
    }
    advance(scanner);
  }

  if (is_at_end(scanner)) {
    return error_token(scanner, "Unterminated string.");
  }

  advance(scanner);
  return make_token(scanner, TOKEN_STRING);
}
