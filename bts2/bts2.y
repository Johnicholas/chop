%token_type {int}

%include {
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

#include "_ceu_app.c"

// BEWARE GLOBALS BEWARE GLOBALS
byte CEU_DATA[sizeof(CEU_Main)];
tceu_app app;
// BEWARE GLOBALS BEWARE GLOBALS

}

%syntax_error {
  fprintf(stderr, "SYNTAX ERROR!\n");
}

start ::= plan.
plan ::= PAR OPEN_PAREN lanes CLOSE_PAREN.
plan ::= SEQ OPEN_PAREN steps CLOSE_PAREN.
plan ::= leaf.
leaf ::= MOVE OPEN_PAREN INT(A) COMMA INT(B) COMMA INT(C) CLOSE_PAREN. {
  tceu__int__int__int payload = { A, B, C };
  ceu_sys_go(&app, CEU_IN_MOVE, &payload);
}
leaf ::= PICKUP OPEN_PAREN INT(A) COMMA INT(B) COMMA INT(C) CLOSE_PAREN. {
  tceu__int__int__int payload = { A, B, C };
  ceu_sys_go(&app, CEU_IN_PICKUP, &payload);
}
leaf ::= DROPOFF OPEN_PAREN INT(A) COMMA INT(B) COMMA INT(C) CLOSE_PAREN. {
  tceu__int__int__int payload = { A, B, C };
  ceu_sys_go(&app, CEU_IN_DROPOFF, &payload);
}
lanes ::= lanes COMMA plan. {
  ceu_sys_go(&app, CEU_IN_PAR, NULL);
}
lanes ::= plan.
steps ::= steps COMMA plan. {
  ceu_sys_go(&app, CEU_IN_SEQ, NULL);
}
steps ::= plan.
