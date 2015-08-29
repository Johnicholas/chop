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
// We need assert.h for assert
#include <assert.h>

// Given a pointer to a member of a struct, the container_of macro allows us
// to back out to the containing struct. Note: this is downcasting.
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({          \
  const typeof(((type *)0)->member)*__mptr = (ptr);    \
  (type *)((char *)__mptr - offsetof(type, member)); })

// utility
state* deepcopy(state* s) {
  int i;

  state* answer = malloc(sizeof(state));
  for (i = 0; i < NUM_BLOCKS; i += 1) {
    answer->pos[i] = s->pos[i];
    answer->clear[i] = s->clear[i];
  }
  answer->holding = s->holding;
  return answer;
}

void print_state(state* to_print) {
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
struct tasks_vtable_ {
  plan* (*seek_plan)(tasks* t, state* s, plan* p, int depth, int v);
};
typedef struct tasks_vtable_ tasks_vtable;
struct tasks_ {
  tasks_vtable* vtable;
};


// An empty plan is a plan
struct empty_plan {
  plan plan;
};
static void method_empty_print(plan* p) {
  struct empty_plan* self = container_of(p, struct empty_plan, plan);
  // printf("empty\n");
}
static plan_vtable empty_plan_vtable = {
  method_empty_print
};
plan* create_empty_plan() {
  struct empty_plan* answer = malloc(sizeof(struct empty_plan));
  answer->plan.vtable = &empty_plan_vtable;
  return &answer->plan;
}


struct pickup_plan {
  plan plan;
  int b;
  plan* rest;
};
static void method_pickup_print(plan* p) {
  struct pickup_plan* self = container_of(p, struct pickup_plan, plan);
  self->rest->vtable->print(self->rest);
  switch (self->b) {
  case A: printf("pickup(A)\n"); break;
  case B: printf("pickup(B)\n"); break;
  case C: printf("pickup(C)\n"); break;
  default: printf("pickup(?)\n"); break;
  }
}
static plan_vtable pickup_plan_vtable = {
  method_pickup_print
};
plan* create_pickup_plan(int b, plan* rest) {
  struct pickup_plan* answer = malloc(sizeof(struct pickup_plan));
  answer->plan.vtable = &pickup_plan_vtable;
  answer->b = b;
  answer->rest = rest;
  return &answer->plan;
}
    

struct unstack_plan {
  plan plan;
  int b;
  int c;
  plan* rest;
};
static void method_unstack_print(plan* p) {
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
static plan_vtable unstack_plan_vtable = {
  method_unstack_print
};
plan* create_unstack_plan(int b, int c, plan* rest) {
  struct unstack_plan* answer = malloc(sizeof(struct unstack_plan));
  answer->plan.vtable = &unstack_plan_vtable;
  answer->b = b;
  answer->c = c;
  answer->rest = rest;
  return &answer->plan;
}
    
struct putdown_plan {
  plan plan;
  int b;
  plan* rest;
};
static void method_putdown_print(plan* p) {
  struct putdown_plan* self = container_of(p, struct putdown_plan, plan);
  self->rest->vtable->print(self->rest);
  switch (self->b) {
  case A: printf("putdown(A)\n"); break;
  case B: printf("putdown(B)\n"); break;
  case C: printf("putdown(C)\n"); break;
  default: printf("putdown(?)\n"); break;
  }
}
static plan_vtable putdown_plan_vtable = {
  method_putdown_print
};
plan* create_putdown_plan(int b, plan* rest) {
  struct putdown_plan* answer = malloc(sizeof(struct putdown_plan));
  answer->plan.vtable = &putdown_plan_vtable;
  answer->b = b;
  answer->rest = rest;
  return &answer->plan;
}


struct stack_plan {
  plan plan;
  int b;
  int c;
  plan* rest;
};
static void method_stack_print(plan* p) {
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
static plan_vtable stack_plan_vtable = {
  method_stack_print
};
plan* create_stack_plan(int b, int c, plan* rest) {
  struct stack_plan* answer = malloc(sizeof(struct stack_plan));
  answer->plan.vtable = &stack_plan_vtable;
  answer->b = b;
  answer->c = c;
  answer->rest = rest;
  return &answer->plan;
}

// blocks world operators

static state* pickup(state* state, int b) {
  if (state->pos[b] == TABLE && state->clear[b] && state->holding == EMPTY) {
    state->pos[b] = HAND;
    state->clear[b] = 0; // FALSE
    state->holding = b;
    return state;
  } else {
    return 0; // NULL
  }
}

static state* unstack(state* state, int b, int c) {
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

static state* putdown(state* state, int b) {
  if (state->pos[b] == HAND) {
    state->pos[b] = TABLE;
    state->clear[b] = 1; // TRUE
    state->holding = EMPTY;
    return state;
  } else {
    return 0; // NULL
  }
}

static state* stack(state* state, int b, int c) {
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
int is_done(int b, state* s, state* goal) {
  if (b == TABLE) {
    return 1; // TRUE
  }
  if (goal->pos[b] != s->pos[b]) { // TODO: b in goal.pos
    return 0; // FALSE
  }
  if (s->pos[b] == TABLE) {
    return 1; // TRUE
  }
  return is_done(s->pos[b], s, goal);
}

enum {
  DONE,
  INACCESSIBLE,
  MOVE_TO_TABLE,
  MOVE_TO_BLOCK,
  WAITING
};

// helper for moveb_m
int status(int b, state* s, state* goal) {
  if (is_done(b, s, goal)) {
    return DONE;
  } else if(!s->clear[b]) {
    return INACCESSIBLE;
  } else if (goal->pos[b] == TABLE) { // TODO b in goal.pos
    return MOVE_TO_TABLE;
  } else if (is_done(goal->pos[b], s, goal) && s->clear[goal->pos[b]]) {
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
tasks* moveb_m(state* s, state* goal, tasks* rest) {
  int b;

  tasks* answer = rest;
  for (b = 0; b < NUM_BLOCKS; b += 1) {
    switch (status(b, s, goal)) {
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
    if (status(b, s, goal) == WAITING) {
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
  tasks tasks;
};
static plan* method_seek_plan_empty(tasks* t,
                                    state* s,
                                    plan* p, 
                                    int depth,
                                    int verbose
) {
  plan* result;
  if (verbose > 1) {
    printf("%s %d: depth %d tasks ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (verbose > 2) {
    printf("%s %d: depth %d plan ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  return p;
}
static tasks_vtable tasks_empty_vtable = {
  method_seek_plan_empty
};
tasks* create_empty() {
  struct empty_tasks* answer = malloc(sizeof(struct empty_tasks));
  answer->tasks.vtable = &tasks_empty_vtable;
  return &answer->tasks;
}

// A list that starts with GET(B) is a kind of list of tasks  
struct get_tasks {
  tasks tasks;
  int b;
  tasks* rest;
};
static plan* method_seek_plan_get(tasks* t,
                                  state* s,
                                  plan* p,
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
  tasks* newtasks = self->rest;
  if (s->clear[self->b]) {
    if (s->pos[self->b] == TABLE) {
      newtasks = create_pickup(self->b, newtasks);
    } else {
      newtasks = create_unstack(self->b, s->pos[self->b], newtasks);
    }
    plan* solution = newtasks->vtable->seek_plan(newtasks, s, p, depth, verbose);
    if (solution) {
      return solution;
    }
  }
  if (verbose > 2) {
    printf("%s %d depth %d returns failure\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  return 0; // NULL
}
static tasks_vtable tasks_get_vtable = {
  method_seek_plan_get
};
tasks* create_get(int b, tasks* rest) {
  struct get_tasks* answer = malloc(sizeof(struct get_tasks));
  answer->tasks.vtable = &tasks_get_vtable;
  answer->b = b;
  answer->rest = rest;
  return &answer->tasks;
}

// A list that starts with PUT(B1, B2) is a kind of list of tasks
struct put_tasks {
  tasks tasks;
  int b1;
  int b2;
  tasks* rest;
};
static plan* method_seek_plan_put(tasks* t,
                                  state* s,
                                  plan* p,
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
  tasks* newtasks = self->rest;
  if (s->holding == self->b1) {
    if (self->b2 == TABLE) {
      newtasks = create_putdown(self->b1, newtasks);
    } else {
      newtasks = create_stack(self->b1, self->b2, newtasks);
    }
    plan* solution = newtasks->vtable->seek_plan(newtasks, s, p, depth, verbose);
    if (solution) {
      return solution;
    }
  }
  if (verbose > 2) {
    printf("%s %d depth %d returns failure\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  return 0; // NULL

}
static tasks_vtable tasks_put_vtable = {
  method_seek_plan_put
};
tasks* create_put(int b1, int b2, tasks* rest) {
  struct put_tasks* answer = malloc(sizeof(struct put_tasks));
  answer->tasks.vtable = &tasks_put_vtable;
  answer->b1 = b1;
  answer->b2 = b2;
  answer->rest = rest;
  return &answer->tasks;
}

// A list that starts with PICKUP(B) is a kind of list of tasks
struct pickup_tasks {
  tasks tasks;
  int b;
  tasks* rest;
};
static plan* method_seek_plan_pickup(tasks* t,
                                     state* s,
                                     plan* p,
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
  state* newstate = pickup(deepcopy(s), self->b);
  if (verbose > 2) {
    printf("%s %d: depth %d new state ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (newstate) {
    plan* solution = self->rest->vtable->seek_plan(self->rest,
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
static tasks_vtable tasks_pickup_vtable = {
  method_seek_plan_pickup
};
tasks* create_pickup(int b, tasks* rest) {
  struct get_tasks* answer = malloc(sizeof(struct get_tasks));
  answer->tasks.vtable = &tasks_pickup_vtable;
  answer->b = b;
  answer->rest = rest;
  return &answer->tasks;
}

// A list that starts with UNSTACK(B, C) is a kind of list of tasks
struct unstack_tasks {
  tasks tasks;
  int b;
  int c;
  tasks* rest;
};
static plan* method_seek_plan_unstack(tasks* t,
                                      state* s,
                                      plan* p,
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
  state* newstate = unstack(deepcopy(s), self->b, self->c);
  if (verbose > 2) {
    printf("%s %d: depth %d new state ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (newstate) {
    plan* solution = self->rest->vtable->seek_plan(self->rest,
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
static tasks_vtable tasks_unstack_vtable = {
  method_seek_plan_unstack
};
tasks* create_unstack(int b, int c, tasks* rest) {
  struct unstack_tasks* answer = malloc(sizeof(struct unstack_tasks));
  answer->tasks.vtable = &tasks_unstack_vtable;
  answer->b = b;
  answer->c = c;
  answer->rest = rest;
  return &answer->tasks;
}

// A list that starts with PUTDOWN(B) is a kind of list of tasks
struct putdown_tasks {
  tasks tasks;
  int b;
  tasks* rest;
};
static plan* method_seek_plan_putdown(tasks* t,
                                      state* s,
                                      plan* p,
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
  state* newstate = putdown(deepcopy(s), self->b);
  if (verbose > 2) {
    printf("%s %d: depth %d new state ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (newstate) {
    plan* solution = self->rest->vtable->seek_plan(self->rest,
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
static tasks_vtable tasks_putdown_vtable = {
  method_seek_plan_putdown
};
tasks* create_putdown(int b, tasks* rest) {
  struct putdown_tasks* answer = malloc(sizeof(struct putdown_tasks));
  answer->tasks.vtable = &tasks_putdown_vtable;
  answer->b = b;
  answer->rest = rest;
  return &answer->tasks;
}

// a list that starts with STACK(B, C) is a kind of list of tasks
struct stack_tasks {
  tasks tasks;
  int b;
  int c;
  tasks* rest;
};
static plan* method_seek_plan_stack(tasks* t,
                                    state* s,
                                    plan* p,
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
  state* newstate = stack(deepcopy(s), self->b, self->c);
  if (verbose > 2) {
    printf("%s %d: depth %d new state ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (newstate) {
    plan* solution = self->rest->vtable->seek_plan(self->rest,
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
static tasks_vtable tasks_stack_vtable = {
  method_seek_plan_stack
};
tasks* create_stack(int b, int c, tasks* rest) {
  struct stack_tasks* answer = malloc(sizeof(struct stack_tasks));
  answer->tasks.vtable = &tasks_stack_vtable;
  answer->b = b;
  answer->c = c;
  answer->rest = rest;
  return &answer->tasks;
}

// a list that starts with MOVE_BLOCKS(goal) is a kind of list of stack
struct move_blocks_tasks {
  tasks tasks;
  state* goal;
  tasks* rest;
};
static plan* method_seek_plan_move_blocks(tasks* t,
                                          state* s,
                                          plan* p,
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
  tasks* newtasks = moveb_m(s, self->goal, self->rest);
  if (verbose > 2) {
    printf("%s %d: depth %d new state ?\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  if (newtasks) {
    plan* solution = newtasks->vtable->seek_plan(newtasks, s, p, depth, verbose);
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
static tasks_vtable tasks_move_blocks_vtable = {
  method_seek_plan_move_blocks
};
tasks* create_move_blocks(state* goal, tasks* rest) {
  struct move_blocks_tasks* answer = malloc(sizeof(struct move_blocks_tasks));
  answer->tasks.vtable = &tasks_move_blocks_vtable;
  answer->goal = goal;
  answer->rest = rest;
  return &answer->tasks;
}

// a list that starts with MOVE_ONE(b1, dest, rest) is a kind of list of stack
struct move_one_tasks {
  tasks tasks;
  int b1;
  int dest;
  tasks* rest;
};
static plan* method_seek_plan_move_one(tasks* t,
                                       state* s,
                                       plan* p,
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
  tasks* newtasks = self->rest;
  newtasks = create_put(self->b1, self->dest, newtasks);
  newtasks = create_get(self->b1, newtasks);
  plan* solution = newtasks->vtable->seek_plan(newtasks, s, p, depth, verbose);
  if (solution) {
    return solution;
  }
  if (verbose > 2) {
    printf("%s %d depth %d returns failure\n", __PRETTY_FUNCTION__, __LINE__, depth);
  }
  return 0; // NULL
}
static tasks_vtable tasks_move_one_vtable = {
  method_seek_plan_move_one
};
tasks* create_move_one(int b1, int dest, tasks* rest) {
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
plan* chop(state* s, tasks* t, int verbose) {
  plan* result;
  if (verbose > 0) {
    printf("** chop, verbose=%d\n", verbose); // TODO: state.__name__, tasks
  }
  result = t->vtable->seek_plan(t, s, create_empty_plan(), 0, verbose);
  if (verbose > 0) {
    printf("** result = ?\n");
  }
  return result;
}

void print_plan(plan* to_print) {
  if (to_print) {
    to_print->vtable->print(to_print);
  } else {
    printf("failed\n");
  }
}
