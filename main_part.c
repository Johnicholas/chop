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

#include "example1.h"
extern void* ParseAlloc(void *(*mallocProc)(size_t));
extern void Parse(void*, int, int);
extern void ParseFree(void*, void (*free)(void*));

int main(int argc, char* argv[]) {
  app.data = (tceu_org*) &CEU_DATA;
  app.init = &ceu_app_init;
  app.init(&app);

  void* p = ParseAlloc(malloc);

  // This corresponds to lexing: "X1 = X2 + X3 * 2"
  Parse(p, ID, 1);
  Parse(p, EQUALS, 0);
  Parse(p, ID, 2);
  Parse(p, PLUS, 0);
  Parse(p, ID, 3);
  Parse(p, TIMES, 0);
  Parse(p, INT, 2);
  Parse(p, 0, 0);
  ceu_sys_go(&app, CEU_IN_PRINT, NULL);

  ParseFree(p, free);
  return 0;
}

