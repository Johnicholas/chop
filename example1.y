%token_type { int }

%left PLUS MINUS.
%left DIVIDE TIMES.

%include {
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "example1.h"
 }

%syntax_error {
  fprintf(stderr, "Syntax error!\n");
 }

program ::= expr(A). { printf("Result=%d\n", A); }

  expr(A) ::= expr(B) MINUS expr(C). { A = B - C; }
    expr(A) ::= expr(B) PLUS expr(C). { A = B + C; }
      expr(A) ::= expr(B) TIMES expr(C). { A = B * C; }
        expr(A) ::= expr(B) DIVIDE expr(C). {
          if (C != 0) {
            A = B / C;
          } else {
            fprintf(stderr, "divide by zero\n");
            exit(1);
          }
        }

          expr(A) ::= INTEGER(B). { A = B; }

