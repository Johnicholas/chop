#include "bts2.h"
#include "bts2.c"

int main(int argc, char* argv[]) {
  app.data = (tceu_org*) &CEU_DATA;
  app.init = &ceu_app_init;
  app.init(&app);

  void* p = ParseAlloc(malloc);
  // This corresponds to lexing: "SEQ(MOVE(1, 2, 3), PICKUP(4, 5, 6), DROPOFF(7, 8, 8))"
  Parse(p, SEQ, 0);
  Parse(p, OPEN_PAREN, 0);
  Parse(p, MOVE, 0);
  Parse(p, OPEN_PAREN, 0);
  Parse(p, INT, 1);
  Parse(p, COMMA, 0);
  Parse(p, INT, 2);
  Parse(p, COMMA, 0);
  Parse(p, INT, 3);
  Parse(p, CLOSE_PAREN, 0);
  Parse(p, COMMA, 0);
  Parse(p, PICKUP, 0);
  Parse(p, OPEN_PAREN, 0);
  Parse(p, INT, 4);
  Parse(p, COMMA, 0);
  Parse(p, INT, 5);
  Parse(p, COMMA, 0);
  Parse(p, INT, 6);
  Parse(p, CLOSE_PAREN, 0);
  Parse(p, COMMA, 0);
  Parse(p, DROPOFF, 0);
  Parse(p, OPEN_PAREN, 0);
  Parse(p, INT, 7);
  Parse(p, COMMA, 0);
  Parse(p, INT, 8);
  Parse(p, COMMA, 0);
  Parse(p, INT, 9);
  Parse(p, CLOSE_PAREN, 0);
  Parse(p, CLOSE_PAREN, 0);
  Parse(p, 0, 0);

  ceu_sys_go(&app, CEU_IN_RUN, NULL);

  ParseFree(p, free);

  return 0;
}

