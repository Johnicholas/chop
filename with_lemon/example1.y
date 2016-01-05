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

assign ::= ID(A) EQUALS sums. {
  tceu__int payload = { A };
  ceu_sys_go(&app, CEU_IN_ASSIGN, &payload);
}
sums ::= sums PLUS products. {
  ceu_sys_go(&app, CEU_IN_PLUS, NULL);
}
sums ::= products.
products ::= products TIMES value. {
  ceu_sys_go(&app, CEU_IN_TIMES, NULL);
}
products ::= value.
value ::= INT(A). {
  tceu__int payload = { A };
  ceu_sys_go(&app, CEU_IN_INTEGER, &payload);
}
value ::= ID(A). {
  tceu__int payload = { A };
  ceu_sys_go(&app, CEU_IN_IDENTIFIER, &payload);
}
