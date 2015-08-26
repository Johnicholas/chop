// Copyright 2015 Johnicholas Hines
//
// This is derived from pyhop.py which was written by Dana S. Nau
//
// Copyright 2013 Dana S. Nau - http://www.cs.umd.edu/~nau
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "chop.h"

// We need stdlib.h for malloc and free and exit
#include <stdlib.h>
// We need stdio.h for printf
#include <stdio.h>

// Given a pointer to a member of a struct, the container_of macro allows us
// to back out to the containing struct. Note: this is downcasting.
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({          \
  const typeof(((type *)0)->member)*__mptr = (ptr);    \
  (type *)((char *)__mptr - offsetof(type, member)); })

// utility
struct state* deepcopy(struct state* s) {
  int i;

  struct state* answer = malloc(sizeof(struct state));
  for (i = 0; i < NUM_BLOCKS; i += 1) {
    answer->pos[i] = s->pos[i];
    answer->clear[i] = s->clear[i];
  }
  answer->holding = s->holding;
  return answer;
}

void print_state(struct state* to_print) {
  int i;

  for (i = 0; i < NUM_BLOCKS; i += 1) {
    if (to_print->pos[i] == TABLE) {
      printf("%c is on the table.\n", 'A'+i);
    } else if (to_print->pos[i] == HAND) {
      printf("%c is in hand.\n", 'A'+i);
    } else {
      printf("%c is on top of %c.\n", 'A'+i, 'A'+to_print->pos[i]);
    }
  }
}

// Tasks is a polymorphic interface, representing a list of tasks
struct tasks_vtable {
  struct plan* (*seek_plan)(struct tasks* t, struct state* s, struct plan* p, int depth, int v);
};
struct tasks {
  struct tasks_vtable* vtable;
};


// An empty plan is a plan
struct empty_plan {
  struct plan plan;
};
static void method_empty_print(struct plan* p) {
  struct empty_plan* self = container_of(p, struct empty_plan, plan);
  // printf("empty\n");
}
static struct plan_vtable empty_plan_vtable = {
  method_empty_print
};
struct plan* create_empty_plan() {
  struct empty_plan* answer = malloc(sizeof(struct empty_plan));
  answer->plan.vtable = &empty_plan_vtable;
  return &answer->plan;
}


struct pickup_plan {
  struct plan plan;
  int b;
  struct plan* rest;
};
static void method_pickup_print(struct plan* p) {
  struct pickup_plan* self = container_of(p, struct pickup_plan, plan);
  self->rest->vtable->print(self->rest);
  switch (self->b) {
  case A: printf("pickup(A)\n"); break;
  case B: printf("pickup(B)\n"); break;
  case C: printf("pickup(C)\n"); break;
  default: printf("pickup(?)\n"); break;
  }
}
static struct plan_vtable pickup_plan_vtable = {
  method_pickup_print
};
struct plan* create_pickup_plan(int b, struct plan* rest) {
  struct pickup_plan* answer = malloc(sizeof(struct pickup_plan));
  answer->plan.vtable = &pickup_plan_vtable;
  answer->b = b;
  answer->rest = rest;
  return &answer->plan;
}
    

struct unstack_plan {
  struct plan plan;
  int b;
  int c;
  struct plan* rest;
};
static void method_unstack_print(struct plan* p) {
  struct unstack_plan* self = container_of(p, struct unstack_plan, plan);
  self->rest->vtable->print(self->rest);
  printf("unstack(");
  switch (self->b) {
  case A: printf("A"); break;
  case B: printf("B"); break;
  case C: printf("C"); break;
  default: printf("?"); break;
  }
  printf(", ");
  switch (self->c) {
  case A: printf("A"); break;
  case B: printf("B"); break;
  case C: printf("C"); break;
  default: printf("?"); break;
  }
  printf(")\n");
}
static struct plan_vtable unstack_plan_vtable = {
  method_unstack_print
};
struct plan* create_unstack_plan(int b, int c, struct plan* rest) {
  struct unstack_plan* answer = malloc(sizeof(struct unstack_plan));
  answer->plan.vtable = &unstack_plan_vtable;
  answer->b = b;
  answer->c = c;
  answer->rest = rest;
  return &answer->plan;
}
    
struct putdown_plan {
  struct plan plan;
  int b;
  struct plan* rest;
};
static void method_putdown_print(struct plan* p) {
  struct putdown_plan* self = container_of(p, struct putdown_plan, plan);
  self->rest->vtable->print(self->rest);
  switch (self->b) {
  case A: printf("putdown(A)\n"); break;
  case B: printf("putdown(B)\n"); break;
  case C: printf("putdown(C)\n"); break;
  default: printf("putdown(?)\n"); break;
  }
}
static struct plan_vtable putdown_plan_vtable = {
  method_putdown_print
};
struct plan* create_putdown_plan(int b, struct plan* rest) {
  struct putdown_plan* answer = malloc(sizeof(struct putdown_plan));
  answer->plan.vtable = &putdown_plan_vtable;
  answer->b = b;
  answer->rest = rest;
  return &answer->plan;
}


struct stack_plan {
  struct plan plan;
  int b;
  int c;
  struct plan* rest;
};
static void method_stack_print(struct plan* p) {
  struct stack_plan* self = container_of(p, struct stack_plan, plan);
  self->rest->vtable->print(self->rest);
  printf("stack(");
  switch (self->b) {
  case A: printf("A"); break;
  case B: printf("B"); break;
  case C: printf("C"); break;
  default: printf("?"); break;
  }
  printf(", ");
  switch (self->c) {
  case A: printf("A"); break;
  case B: printf("B"); break;
  case C: printf("C"); break;
  default: printf("?"); break;
  }
  printf(")\n");
}
static struct plan_vtable stack_plan_vtable = {
  method_stack_print
};
struct plan* create_stack_plan(int b, int c, struct plan* rest) {
  struct stack_plan* answer = malloc(sizeof(struct stack_plan));
  answer->plan.vtable = &stack_plan_vtable;
  answer->b = b;
  answer->c = c;
  answer->rest = rest;
  return &answer->plan;
}

// blocks world operators

static struct state* pickup(struct state* state, int b) {
  if (state->pos[b] == TABLE && state->clear[b] && state->holding == EMPTY) {
    state->pos[b] = HAND;
    state->clear[b] = 0; // FALSE
    state->holding = b;
    return state;
  } else {
    return 0; // NULL
  }
}

static struct state* unstack(struct state* state, int b, int c) {
  if (state->pos[b] == c && c != TABLE && state->clear[b] && state->holding == EMPTY) {
    state->pos[b] = HAND;
    state->clear[b] = 0; // FALSE
    state->holding = b;
    state->clear[c] = 1; // TRUE
    return state;
  } else {
    return 0; // NULL
  }
}

static struct state* putdown(struct state* state, int b) {
  if (state->pos[b] == HAND) {
    state->pos[b] = TABLE;
    state->clear[b] = 1; // TRUE
    state->holding = EMPTY;
    return state;
  } else {
    return 0; // NULL
  }
}

static struct state* stack(struct state* state, int b, int c) {
  if (state->pos[b] == HAND && state->clear[c]) {
    state->pos[b] = c;
    state->clear[b] = 1; // TRUE
    state->holding = EMPTY;
    state->clear[c] = 0; // FALSE
    return state;
  } else {
    return 0; // NULL
  }
}

// blocks world methods

// helper for status
int is_done(int b, struct state* state, struct state* goal) {
  if (b == TABLE) {
    return 1; // TRUE
  }
  if (goal->pos[b] != state->pos[b]) { // TODO: b in goal.pos
    return 0; // FALSE
  }
  if (state->pos[b] == TABLE) {
    return 1; // TRUE
  }
  return is_done(state->pos[b], state, goal);
}

enum {
  DONE,
  INACCESSIBLE,
  MOVE_TO_TABLE,
  MOVE_TO_BLOCK,
  WAITING
};

// helper for moveb_m
int status(int b, struct state* state, struct state* goal) {
  if (is_done(b, state, goal)) {
    return DONE;
  } else if(!state->clear[b]) {
    return INACCESSIBLE;
  } else if (goal->pos[b] == TABLE) { // TODO b in goal.pos
    return MOVE_TO_TABLE;
  } else if (is_done(goal->pos[b], state, goal) && state->clear[goal->pos[b]]) {
    return MOVE_TO_BLOCK;
  } else {
    return WAITING;
  }
}

// This method implements the following block-stacking algorithm:
// If there's a block that can be moved to its final position, then
// do so and call move_blocks recursively. Otherwise, if there's a
// block that needs to b emoved and can be moved to the table, then
// do so and call move_blocks recursively. Otherwise, no blocks need
// to be moved.
struct tasks* moveb_m(struct state* state, struct state* goal, struct tasks* rest) {
  int b;

  struct tasks* answer = rest;
  for (b = 0; b < NUM_BLOCKS; b += 1) {
    switch (status(b, state, goal)) {
    case MOVE_TO_TABLE:
      // return [(MOVE_ONE, b, TABLE), (MOVE_BLOCKS, goal)];
      answer = create_move_blocks(goal, answer);
      answer = create_move_one(b, TABLE, answer);
      return answer;
    case MOVE_TO_BLOCK:
      // return [(MOVE_ONE, b, goal->pos[b]), (MOVE_BLOCKS, goal)];
      answer = create_move_blocks(goal, answer);
      answer = create_move_one(b, goal->pos[b], answer);
      return answer;
    default:
      continue;
    }
  }
  // if we get here, no blocks can be moved to their final locations
  for (b = 0; b < NUM_BLOCKS; b += 1) {
    if (status(b, state, goal) == WAITING) {
      // return [(MOVE_ONE, b, TABLE), (MOVE_BLOCKS, goal)];
      answer = create_move_blocks(goal, answer);
      answer = create_move_one(b, TABLE, answer);
      return answer;
    }
  }
  // if we get here, there are no blocks that need moving
  // return []
  return answer;
}


// The empty list is a kind of list of tasks
struct empty_tasks {
  struct tasks tasks;
};
static struct plan* method_seek_plan_empty(struct tasks* t,
                                           struct state* s,
                                           struct plan* p, 
                                           int depth,
                                           int verbose
) {
  struct plan* result;
  if (verbose > 1) {
    printf("%s %d: depth %d tasks ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (verbose > 2) {
    printf("%s %d: depth %d plan ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  return p;
}
static struct tasks_vtable tasks_empty_vtable = {
  method_seek_plan_empty
};
struct tasks* create_empty() {
  struct empty_tasks* answer = malloc(sizeof(struct empty_tasks));
  answer->tasks.vtable = &tasks_empty_vtable;
  return &answer->tasks;
}

// A list that starts with GET(B) is a kind of list of tasks  
struct get_tasks {
  struct tasks tasks;
  int b;
  struct tasks* rest;
};
static struct plan* method_seek_plan_get(struct tasks* t,
                                         struct state* s,
                                         struct plan* p,
                                         int depth,
                                         int verbose
) {
  struct get_tasks* self = container_of(t, struct get_tasks, tasks);
  if (verbose > 1) {
    printf("%s %d: depth %d tasks ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (verbose > 2) {
    printf("%s %d: depth %d method instance get\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  // generate either a pickup or an unstack subtask for b
  struct tasks* newtasks = self->rest;
  if (s->clear[self->b]) {
    if (s->pos[self->b] == TABLE) {
      newtasks = create_pickup(self->b, newtasks);
    } else {
      newtasks = create_unstack(self->b, s->pos[self->b], newtasks);
    }
    struct plan* solution = newtasks->vtable->seek_plan(newtasks, s, p, depth, verbose);
    if (solution) {
      return solution;
    }
  }
  if (verbose > 2) {
    printf("%s %d depth %d returns failure\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  return 0; // NULL
}
static struct tasks_vtable tasks_get_vtable = {
  method_seek_plan_get
};
struct tasks* create_get(int b, struct tasks* rest) {
  struct get_tasks* answer = malloc(sizeof(struct get_tasks));
  answer->tasks.vtable = &tasks_get_vtable;
  answer->b = b;
  answer->rest = rest;
  return &answer->tasks;
}

// A list that starts with PUT(B1, B2) is a kind of list of tasks
struct put_tasks {
  struct tasks tasks;
  int b1;
  int b2;
  struct tasks* rest;
};
static struct plan* method_seek_plan_put(struct tasks* t,
                                         struct state* s,
                                         struct plan* p,
                                         int depth,
                                         int verbose
) {
  struct put_tasks* self = container_of(t, struct put_tasks, tasks);
  if (verbose > 1) {
    printf("%s %d: depth %d tasks ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (verbose > 2) {
    printf("%s %d: depth %d method instance put\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  // generate either a putdown or stack subtask for b1
  // b2 is b1's destination: either the table or another block
  struct tasks* newtasks = self->rest;
  if (s->holding == self->b1) {
    if (self->b2 == TABLE) {
      newtasks = create_putdown(self->b1, newtasks);
    } else {
      newtasks = create_stack(self->b1, self->b2, newtasks);
    }
    struct plan* solution = newtasks->vtable->seek_plan(newtasks, s, p, depth, verbose);
    if (solution) {
      return solution;
    }
  }
  if (verbose > 2) {
    printf("%s %d depth %d returns failure\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  return 0; // NULL

}
static struct tasks_vtable tasks_put_vtable = {
  method_seek_plan_put
};
struct tasks* create_put(int b1, int b2, struct tasks* rest) {
  struct put_tasks* answer = malloc(sizeof(struct put_tasks));
  answer->tasks.vtable = &tasks_put_vtable;
  answer->b1 = b1;
  answer->b2 = b2;
  answer->rest = rest;
  return &answer->tasks;
}

// A list that starts with PICKUP(B) is a kind of list of tasks
struct pickup_tasks {
  struct tasks tasks;
  int b;
  struct tasks* rest;
};
static struct plan* method_seek_plan_pickup(struct tasks* t,
                                            struct state* s,
                                            struct plan* p,
                                            int depth,
                                            int verbose
) {
  struct pickup_tasks* self = container_of(t, struct pickup_tasks, tasks);
  if (verbose > 1) {
    printf("%s %d: depth %d tasks ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (verbose > 2) {
    printf("%s %d: depth %d action pickup\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  struct state* newstate = pickup(deepcopy(s), self->b);
  if (verbose > 2) {
    printf("%s %d: depth %d new state ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (newstate) {
    struct plan* solution = self->rest->vtable->seek_plan(self->rest,
                                                          newstate,
                                                          create_pickup_plan(self->b, p),
                                                          depth,
                                                          verbose);
    if (solution) {
      return solution;
    }
  }
  if (verbose > 2) {
    printf("%s %d depth %d returns failure\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  return 0; // NULL
}
static struct tasks_vtable tasks_pickup_vtable = {
  method_seek_plan_pickup
};
struct tasks* create_pickup(int b, struct tasks* rest) {
  struct get_tasks* answer = malloc(sizeof(struct get_tasks));
  answer->tasks.vtable = &tasks_pickup_vtable;
  answer->b = b;
  answer->rest = rest;
  return &answer->tasks;
}

// A list that starts with UNSTACK(B, C) is a kind of list of tasks
struct unstack_tasks {
  struct tasks tasks;
  int b;
  int c;
  struct tasks* rest;
};
static struct plan* method_seek_plan_unstack(struct tasks* t,
                                             struct state* s,
                                             struct plan* p,
                                             int depth,
                                             int verbose
) {
  struct unstack_tasks* self = container_of(t, struct unstack_tasks, tasks);
  if (verbose > 1) {
    printf("%s %d: depth %d tasks ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (verbose > 2) {
    printf("%s %d: depth %d action unstack\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  struct state* newstate = unstack(deepcopy(s), self->b, self->c);
  if (verbose > 2) {
    printf("%s %d: depth %d new state ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (newstate) {
    struct plan* solution = self->rest->vtable->seek_plan(self->rest,
                                                          newstate,
                                                          create_unstack_plan(self->b, self->c, p),
                                                          depth,
                                                          verbose);
    if (solution) {
      return solution;
    }
  }
  if (verbose > 2) {
    printf("%s %d depth %d returns failure\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  return 0; // NULL
}
static struct tasks_vtable tasks_unstack_vtable = {
  method_seek_plan_unstack
};
struct tasks* create_unstack(int b, int c, struct tasks* rest) {
  struct unstack_tasks* answer = malloc(sizeof(struct unstack_tasks));
  answer->tasks.vtable = &tasks_unstack_vtable;
  answer->b = b;
  answer->c = c;
  answer->rest = rest;
  return &answer->tasks;
}

// A list that starts with PUTDOWN(B) is a kind of list of tasks
struct putdown_tasks {
  struct tasks tasks;
  int b;
  struct tasks* rest;
};
static struct plan* method_seek_plan_putdown(struct tasks* t,
                                             struct state* s,
                                             struct plan* p,
                                             int depth,
                                             int verbose
) {
  struct putdown_tasks* self = container_of(t, struct putdown_tasks, tasks);
  if (verbose > 1) {
    printf("%s %d: depth %d tasks ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (verbose > 2) {
    printf("%s %d: depth %d action putdown\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  struct state* newstate = putdown(deepcopy(s), self->b);
  if (verbose > 2) {
    printf("%s %d: depth %d new state ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (newstate) {
    struct plan* solution = self->rest->vtable->seek_plan(self->rest,
                                                          newstate,
                                                          create_putdown_plan(self->b, p),
                                                          depth,
                                                          verbose);
    if (solution) {
      return solution;
    }
  }
  if (verbose > 2) {
    printf("%s %d depth %d returns failure\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  return 0; // NULL
}
static struct tasks_vtable tasks_putdown_vtable = {
  method_seek_plan_putdown
};
struct tasks* create_putdown(int b, struct tasks* rest) {
  struct putdown_tasks* answer = malloc(sizeof(struct putdown_tasks));
  answer->tasks.vtable = &tasks_putdown_vtable;
  answer->b = b;
  answer->rest = rest;
  return &answer->tasks;
}

// a list that starts with STACK(B, C) is a kind of list of tasks
struct stack_tasks {
  struct tasks tasks;
  int b;
  int c;
  struct tasks* rest;
};
static struct plan* method_seek_plan_stack(struct tasks* t,
                                           struct state* s,
                                           struct plan* p,
                                           int depth,
                                           int verbose
) {
  struct stack_tasks* self = container_of(t, struct stack_tasks, tasks);
  if (verbose > 1) {
    printf("%s %d: depth %d tasks ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (verbose > 2) {
    printf("%s %d: depth %d action stack\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  struct state* newstate = stack(deepcopy(s), self->b, self->c);
  if (verbose > 2) {
    printf("%s %d: depth %d new state ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (newstate) {
    struct plan* solution = self->rest->vtable->seek_plan(self->rest,
                                                          newstate,
                                                          create_stack_plan(self->b, self->c, p),
                                                          depth,
                                                          verbose);
    if (solution) {
      return solution;
    }
  }
  if (verbose > 2) {
    printf("%s %d depth %d returns failure\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  return 0; // NULL
}
static struct tasks_vtable tasks_stack_vtable = {
  method_seek_plan_stack
};
struct tasks* create_stack(int b, int c, struct tasks* rest) {
  struct stack_tasks* answer = malloc(sizeof(struct stack_tasks));
  answer->tasks.vtable = &tasks_stack_vtable;
  answer->b = b;
  answer->c = c;
  answer->rest = rest;
  return &answer->tasks;
}

// a list that starts with MOVE_BLOCKS(goal) is a kind of list of stack
struct move_blocks_tasks {
  struct tasks tasks;
  struct state* goal;
  struct tasks* rest;
};
static struct plan* method_seek_plan_move_blocks(struct tasks* t,
                                                 struct state* s,
                                                 struct plan* p,
                                                 int depth,
                                                 int verbose
) {
  struct move_blocks_tasks* self = container_of(t, struct move_blocks_tasks, tasks);
  if (verbose > 1) {
    printf("%s %d: depth %d tasks ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (verbose > 2) {
    printf("%s %d: depth %d method instance move blocks\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  struct tasks* newtasks = moveb_m(s, self->goal, self->rest);
  if (verbose > 2) {
    printf("%s %d: depth %d new state ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (newtasks) {
    struct plan* solution = newtasks->vtable->seek_plan(newtasks, s, p, depth, verbose);
    if (solution) {
      return solution;
    } else {
      printf("%s %d: no solution\n", __PRETTY_FUNCTION__, __LINE__);
    }
  }
  if (verbose > 2) {
    printf("%s %d depth %d returns failure\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  return 0; // NULL
}
static struct tasks_vtable tasks_move_blocks_vtable = {
  method_seek_plan_move_blocks
};
struct tasks* create_move_blocks(struct state* goal, struct tasks* rest) {
  struct move_blocks_tasks* answer = malloc(sizeof(struct move_blocks_tasks));
  answer->tasks.vtable = &tasks_move_blocks_vtable;
  answer->goal = goal;
  answer->rest = rest;
  return &answer->tasks;
}

// a list that starts with MOVE_ONE(b1, dest, rest) is a kind of list of stack
struct move_one_tasks {
  struct tasks tasks;
  int b1;
  int dest;
  struct tasks* rest;
};
static struct plan* method_seek_plan_move_one(struct tasks* t,
                                              struct state* s,
                                              struct plan* p,
                                              int depth,
                                              int verbose
) {
  struct move_one_tasks* self = container_of(t, struct move_one_tasks, tasks);
  if (verbose > 1) {
    printf("%s %d: depth %d tasks ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (verbose > 2) {
    printf("%s %d: depth %d method instance get\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  // generate subtasks to get b1 and put it at dest
  // [(GET, b1), (PUT, b1, dest)]
  struct tasks* newtasks = self->rest;
  newtasks = create_put(self->b1, self->dest, newtasks);
  newtasks = create_get(self->b1, newtasks);
  struct plan* solution = newtasks->vtable->seek_plan(newtasks, s, p, depth, verbose);
  if (solution) {
    return solution;
  }
  if (verbose > 2) {
    printf("%s %d depth %d returns failure\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  return 0; // NULL
}
static struct tasks_vtable tasks_move_one_vtable = {
  method_seek_plan_move_one
};
struct tasks* create_move_one(int b1, int dest, struct tasks* rest) {
  struct move_one_tasks* answer = malloc(sizeof(struct move_one_tasks));
  answer->tasks.vtable = &tasks_move_one_vtable;
  answer->b1 = b1;
  answer->dest = dest;
  answer->rest = rest;
  return &answer->tasks;
}


// blocks world methods

// Try to find a plan that accomplishes tasks t starting from state s
// If successful, return the plan, otherwise return null.
struct plan* chop(struct state* s, struct tasks* t, int verbose) {
  struct plan* result;
  if (verbose > 0) {
    printf("** chop, verbose=%d\n", verbose); // TODO: state.__name__, tasks
  }
  result = t->vtable->seek_plan(t, s, create_empty_plan(), 0, verbose);
  if (verbose > 0) {
    printf("** result = ?\n");
  }
  return result;
}



  

