#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define ceu_out_assert(v) ceu_sys_assert(v)
void ceu_sys_assert (int v) {
  assert(v);
}

#define ceu_out_log(m,s) ceu_sys_log(m,s)
void ceu_sys_log (int mode, long s) {
  switch (mode) {
  case 0:
    printf("%s", (char*)s);
    break;
  case 1:
    printf("%lX", s);
    break;
  case 2:
    printf("%ld", s);
    break;
  }
}

typedef int* intptr;

#include "_ceu_app.c"

// BEWARE GLOBALS BEWARE GLOBALS
byte CEU_DATA[sizeof(CEU_Main)];
tceu_app app;
// BEWARE GLOBALS BEWARE GLOBALS

int main(int argc, char* argv[]) {
  app.data = (tceu_org*) &CEU_DATA;
  app.init = &ceu_app_init;
  app.init(&app);

  // This corresponds to lexing: "X1 = X2 + X3 * 2"

  ceu_sys_go(&app, CEU_IN_PRINT, NULL);
  // spawn Identifier 1
  {
    tceu__int payload = { 2 };
    ceu_sys_go(&app, CEU_IN_IDENTIFIER, &payload);
  }
  ceu_sys_go(&app, CEU_IN_PRINT, NULL);
  // spawn Identifier
  {
    tceu__int payload = { 3 };
    ceu_sys_go(&app, CEU_IN_IDENTIFIER, &payload);
  }
  ceu_sys_go(&app, CEU_IN_PRINT, NULL);
  // spawn Integer
  {
    tceu__int payload = { 2 };
    ceu_sys_go(&app, CEU_IN_INTEGER, &payload);
  }
  ceu_sys_go(&app, CEU_IN_PRINT, NULL);
  // spawn Times
  {
    ceu_sys_go(&app, CEU_IN_TIMES, NULL );
  }
  ceu_sys_go(&app, CEU_IN_PRINT, NULL);
  // spawn Plus
  {
    ceu_sys_go(&app, CEU_IN_PLUS, NULL );
  }
  ceu_sys_go(&app, CEU_IN_PRINT, NULL);
  // spawn Assign
  {
    tceu__int payload = { 1 };
    ceu_sys_go(&app, CEU_IN_ASSIGN, &payload);
  }

  ceu_sys_go(&app, CEU_IN_PRINT, NULL);

  return 0;
}

