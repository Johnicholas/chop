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

struct state {
  int pos[NUM_BLOCKS]; // s->pos[A] answers "where is A?"
  int clear[NUM_BLOCKS]; // s->clear[A] answers "is A clear?"
  int holding; // s->holding answers "what are we currently holding?"
};

void print_state(struct state* to_print);

// forward declaration
struct tasks;

struct tasks* create_empty();
struct tasks* create_get(int b, struct tasks* rest);
struct tasks* create_put(int b1, int b2, struct tasks* rest);
struct tasks* create_pickup(int b, struct tasks* rest);
struct tasks* create_unstack(int b, int c, struct tasks* rest);
struct tasks* create_putdown(int b, struct tasks* rest);
struct tasks* create_stack(int b, int c, struct tasks* rest);
struct tasks* create_move_blocks(struct state* goal, struct tasks* rest);
struct tasks* create_move_one(int b1, int dest, struct tasks* rest);

struct plan;
struct plan_vtable {
  void (*print)(struct plan* self);
};
struct plan {
  struct plan_vtable* vtable;
};

struct plan* chop(struct state* s, struct tasks* t, int verbose);

#endif

