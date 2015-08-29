#include "chop.h"

// We need stdio.h for printf
#include <stdio.h>

int main(int argc, char* argv[]) {
  printf("First, test chop on some of the operators and smaller tasks\n");
  printf("- Define state1:\n");
  struct state state1 = {
    { B, TABLE, TABLE }, // pos
    { 1, 0, 1 }, // clear
    EMPTY // holding
  };
  print_state(&state1);

  printf("- These should fail:\n");
  // [(PICKUP, A)];
  struct tasks* pickup_a = create_empty();
  pickup_a = create_pickup(A, pickup_a);
  struct plan* pickup_a_out = chop(&state1, pickup_a, 0);
  if (pickup_a_out) {
    pickup_a_out->vtable->print(pickup_a_out);
  } else {
    printf("failed\n");
  }

  // [(PICKUP, B)];
  struct tasks* pickup_b = create_empty();
  pickup_b = create_pickup(B, pickup_a);
  struct plan* pickup_b_out = chop(&state1, pickup_b, 0);
  if (pickup_b_out) {
    pickup_b_out->vtable->print(pickup_b_out);
  } else {
    printf("failed\n");
  }

  printf("- these should succeed:\n");

  // [(PICKUP, C)];
  struct tasks* pickup_c = create_empty();
  pickup_c = create_pickup(C, pickup_c);
  struct plan* pickup_c_out = chop(&state1, pickup_c, 0);
  if (pickup_c_out) {
    pickup_c_out->vtable->print(pickup_c_out);
  } else {
    printf("failed\n");
  }

  // [(UNSTACK, A, B)];
  struct tasks* unstack_a_b = create_empty();
  unstack_a_b = create_unstack(A, B, unstack_a_b);
  struct plan* unstack_a_b_out = chop(&state1, unstack_a_b, 0);
  if (unstack_a_b_out) {
    unstack_a_b_out->vtable->print(unstack_a_b_out);
  } else {
    printf("failed\n");
  }

  printf("Run chop on two block-stacking problems, both of which start in state1.\n");
  printf("The goal for the 2nd problem omits some of the conditions in the goal\n");
  printf("of the 1st problem, but those conditions will need to be achieved\n");
  printf("anyway, so both goals should produce the same plan.\n");
  printf("- Define goal1a:\n");
  struct state goal1a = {
    { TABLE, A, B }, // pos
    { 0, 0, 1 }, // clear
    EMPTY // holding
  };
  print_state(&goal1a); // TODO: print_goal
  printf("\n");
  /*
  printf("- Define goal1b:");
  struct state goal1b = {
    { ???, A, B }, // pos
    { ???, ???, ??? }, // clear
    ??? // holding
  };
  */
  // [(MOVE_BLOCKS, goal1a)];
  struct tasks* move_blocks_goal1a = create_empty();
  move_blocks_goal1a = create_move_blocks(&goal1a, move_blocks_goal1a);
  struct plan* move_blocks_goal1a_out = chop(&state1, move_blocks_goal1a, 0);
  if (move_blocks_goal1a_out) {
    move_blocks_goal1a_out->vtable->print(move_blocks_goal1a_out);
  } else {
    printf("failed\n");
  }

  return 0;
}
