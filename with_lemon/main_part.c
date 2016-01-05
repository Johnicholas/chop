#include "example1.h"
#include "example1.c"

int main(int argc, char* argv[]) {
  app.data = (tceu_org*) &CEU_DATA;
  app.init = &ceu_app_init;
  app.init(&app);

  void* p = ParseAlloc(malloc);
  // This corresponds to lexing: "X1 = X2 + X3 * 2"
  ceu_sys_go(&app, CEU_IN_PRINT, NULL);
  Parse(p, ID, 1);
  ceu_sys_go(&app, CEU_IN_PRINT, NULL);
  Parse(p, EQUALS, 0);
  ceu_sys_go(&app, CEU_IN_PRINT, NULL);
  Parse(p, ID, 2);
  ceu_sys_go(&app, CEU_IN_PRINT, NULL);
  Parse(p, PLUS, 0);
  ceu_sys_go(&app, CEU_IN_PRINT, NULL);
  Parse(p, ID, 3);
  ceu_sys_go(&app, CEU_IN_PRINT, NULL);
  Parse(p, TIMES, 0);
  ceu_sys_go(&app, CEU_IN_PRINT, NULL);
  Parse(p, INT, 2);
  ceu_sys_go(&app, CEU_IN_PRINT, NULL);
  Parse(p, 0, 0);
  ceu_sys_go(&app, CEU_IN_PRINT, NULL);

  ParseFree(p, free);

  return 0;
}

