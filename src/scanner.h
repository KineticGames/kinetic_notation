#ifndef KINETIC_NOTATION_SCANNER_H
#define KINETIC_NOTATION_SCANNER_H

typedef enum {
  // Single-character tokens.
  TOKEN_LEFT_BRACKET,
  TOKEN_RIGHT_BRACKET,
  TOKEN_LEFT_BRACE,
  TOKEN_RIGHT_BRACE,
  TOKEN_COLON,
  TOKEN_NEWLINE,
  // Literals.
  TOKEN_IDENTIFIER,
  TOKEN_STRING,
  TOKEN_NUMBER,
  TOKEN_VERSION,
  // Keywords.
  TOKEN_FALSE,
  TOKEN_TRUE,
  // Other.
  TOKEN_ERROR,
  TOKEN_EOF
} TokenType;

typedef struct {
  TokenType type;
  const char *start;
  int length;
  int line;
} Token;

typedef struct Scanner *Scanner;

Scanner scanner_init(const char *source);
Token scanner_scan_token(Scanner scanner);

#endif // KINETIC_NOTATION_SCANNER_H
