#include "parser.h"

#include "scanner.h"

// std

typedef struct Parser {
  Scanner scanner;
  Token current;
  Token previous;
} Parser;

static void advance(Parser parser);

KnResult parse(KnStructure structure, const char *source) {
  Parser parser;
  parser.scanner = scanner_init(source);

  return SUCCESS;
}

static void advance(Parser parser) {
  parser.previous = parser.current;

  for (;;) {
    parser.current = scanner_scan_token(parser.scanner);

    if (parser.current.type != TOKEN_ERROR) {
      break;
    }

    error_at_current(parser.current.start);
  }
}
