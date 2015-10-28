#ifndef CHOP_H
#define CHOP_H

// blocks world state

enum {
  A,
  B,
  C,
  TABLE,
  HAND,
  EMPTY,
};
#define NUM_BLOCKS 3

struct state_ {
  int pos[NUM_BLOCKS]; // s->pos[A] answers "where is A?"
  int clear[NUM_BLOCKS]; // s->clear[A] answers "is A clear?"
  int holding; // s->holding answers "what are we currently holding?"
};
typedef struct state_ state;

void print_state(state* to_print);

// forward declaration
struct tasks_;
typedef struct tasks_ tasks;
typedef tasks* tasks_ptr;

tasks_ptr create_empty();
tasks_ptr create_get(int b, tasks_ptr rest);
tasks_ptr create_put(int b1, int b2, tasks_ptr rest);
tasks_ptr create_pickup(int b, tasks_ptr rest);
tasks_ptr create_unstack(int b, int c, tasks_ptr rest);
tasks_ptr create_putdown(int b, tasks_ptr rest);
tasks_ptr create_stack(int b, int c, tasks_ptr rest);
tasks_ptr create_move_blocks(state* goal, tasks_ptr rest);
tasks_ptr create_move_one(int b1, int dest, tasks_ptr rest);

struct plan_;
typedef struct plan_ plan;
struct plan_vtable_ {
  void (*print)(plan* self);
};
typedef struct plan_vtable_ plan_vtable;
struct plan_ {
  plan_vtable* vtable;
};

plan* chop(state* s, tasks_ptr t, int verbose);
void print_plan(plan*);

#endif

