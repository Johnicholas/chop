#include "chop.h"
#include "_ceu_app.h"

// We need stdio.h for printf
#include <stdio.h>

#define countof(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))

int main(int argc, char* argv[]) {
  struct model state1;
  // The map is E-shaped.
  // We express the map via a single "parent",
  // for every location except the root,
  // creating a spanning tree.
  //
  // +<+<+
  // v
  // +<+<+
  // ^
  // +<+<+
  //
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
  state1.pos[P4] = E;
  state1.pos[P5] = SE;
  state1.pos[P6] = S;
  state1.pos[P7] = SW;
  state1.pos[P8] = W;
  state1.pos[T1] = C;
  state1.pos[T2] = C;
  state1.pos[T3] = C;

  int truck_list[] = { T1, T2, T3 };
  state1.trucks = truck_list;
  state1.number_of_trucks = countof(truck_list);

  struct goal goal1;
  goal1.pos[P1] = W;
  goal1.pos[P2] = NW;
  goal1.pos[P3] = N;
  goal1.pos[P4] = NE;
  goal1.pos[P5] = E;
  goal1.pos[P6] = SE;
  goal1.pos[P7] = S;
  goal1.pos[P8] = SW;
  goal1.pos[T1] = DONT_CARE;
  goal1.pos[T2] = DONT_CARE;
  goal1.pos[T3] = DONT_CARE;

  struct tceu_app* out = chop(&state1, &goal1);

  printf("done planning, starting to execute the plan\n");

  ceu_sys_go(out, CEU_IN_START, (void*)0);
  int i;
  for (i = 0; i < 25; i++) {
    printf("...\n");
    ceu_sys_go(out, CEU_IN_TICK, (void*)0);
  }

  return 0;
}
