%include {

#include <assert.h>

// I don't think these macros matter here?
// because we just need the enums and typedefs
#define ceu_out_assert(X)
#define ceu_out_log(X)
typedef int* intptr;
#include "_ceu_app.h"

extern tceu_app app;

}

%token_type { int }

%syntax_error {
  fprintf(stderr, "Syntax error!\n");
}

assign(A) ::= ID(B) EQUALS sums(C). { 
  tceu__intptr__int__int payload = { &A, B, C };
  ceu_sys_go(&app, CEU_IN_ASSIGN, &payload);
}
sums(A) ::= sums(B) PLUS products(C). {
  tceu__intptr__int__int payload = { &A, B, C };
  ceu_sys_go(&app, CEU_IN_SUMS_PLUS, &payload);
}
sums(A) ::= products(B). {

  tceu__intptr__int payload = { &A, B };
  ceu_sys_go(&app, CEU_IN_SUMS, &payload);
}  
products(A) ::= products(B) TIMES value(C). {
  tceu__intptr__int__int payload = { &A, B, C };
  ceu_sys_go(&app, CEU_IN_PRODUCTS_TIMES, &payload);
}
products(A) ::= value(B). {
  tceu__intptr__int payload = { &A, B };
  ceu_sys_go(&app, CEU_IN_PRODUCTS, &payload);
}
value(A) ::= INT(B). {
  tceu__intptr__int payload = { &A, B };
  ceu_sys_go(&app, CEU_IN_VALUE_INT, &payload);
}    
value(A) ::= ID(B). {
  tceu__intptr__int payload = { &A, B };
  ceu_sys_go(&app, CEU_IN_VALUE_ID, &payload);
}

