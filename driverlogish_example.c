#include "chop.h"
#include "_ceu_app.h"

// We need stdio.h for printf
#include <stdio.h>

#define countof(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))

int main(int argc, char* argv[]) {
  struct model state1;

  // This map is shaped like an E
  state1.route[NW] = W;
  state1.route[N] = NW;
  state1.route[NE] = N;
  state1.route[W] = ROOT;
  state1.route[C] = W;
  state1.route[E] = C;
  state1.route[SW] = W;
  state1.route[S] = SW;
  state1.route[SE] = S;

  state1.pos[P1] = NW;
  state1.pos[P2] = N;
  state1.pos[P3] = NE;
  state1.pos[P4] = W;
  state1.pos[P5] = C;
  state1.pos[P6] = E;
  state1.pos[P7] = SW;
  state1.pos[P8] = S;
  state1.pos[P9] = SE;
  state1.pos[T1] = C;
  state1.pos[T2] = C;
  state1.pos[T3] = C;

  int truck_list[] = { T1, T2, T3 };
  state1.trucks = truck_list;
  state1.number_of_trucks = countof(truck_list);

  struct goal goal1;
  goal1.pos[P1] = C;
  goal1.pos[P2] = C;
  goal1.pos[P3] = C;
  goal1.pos[P4] = C;
  goal1.pos[P5] = C;
  goal1.pos[P6] = C;
  goal1.pos[P7] = C;
  goal1.pos[P8] = C;
  goal1.pos[P9] = C;

  goal1.pos[T1] = DONT_CARE;
  goal1.pos[T2] = DONT_CARE;
  goal1.pos[T3] = DONT_CARE;

  struct tceu_app* out = chop(&state1, &goal1);

  printf("done planning, starting to execute the plan\n");

  ceu_sys_go(out, CEU_IN_START, (void*)0);
  int i;
  for (i = 100; i < 200; i++) {
    printf("...\n");
    ceu_sys_go(out, CEU_IN_TICK, &i);
  }

  return 0;
}
