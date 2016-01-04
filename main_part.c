#include <stdlib.h>

#include "example1.h"

extern void* ParseAlloc(void *(*mallocProc)(size_t));
extern void Parse(void*, int, int);
extern void ParseFree(void*, void (*free)(void*));

int main(int argc, char* argv[]) {
  void* p = ParseAlloc(malloc);

  // This corresponds to lexing: "15 / 5"
  Parse(p, INTEGER, 15);
  Parse(p, DIVIDE, 0);
  Parse(p, INTEGER, 5);
  Parse(p, 0, 0);

  // This corresponds to lexing: "10 + 125"
  Parse(p, INTEGER, 50);
  Parse(p, PLUS, 0);
  Parse(p, INTEGER, 125);
  Parse(p, 0, 0);

  // This corresponds to lexing: "50 * 125 + 125"
  Parse(p, INTEGER, 50);
  Parse(p, TIMES, 0);
  Parse(p, INTEGER, 125);
  Parse(p, PLUS, 0);
  Parse(p, INTEGER, 125);
  Parse(p, 0, 0);

  ParseFree(p, free);
}

