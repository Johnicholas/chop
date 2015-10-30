#include "chop.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "_ceu_app.c"

#undef offsetof
// Given a pointer to a member of a struct, the container_of macro allows us
// to back out to the containing struct. Note: this is downcasting.
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({          \
  const typeof(((type *)0)->member)*__mptr = (ptr);    \
  (type *)((char *)__mptr - offsetof(type, member)); })

struct planning_state {
  int route[NUMBER_OF_LOCATIONS];
  int pos[NUMBER_OF_ENTITIES];
  int *trucks;
  int number_of_trucks;
  int gensym;
  int now[NUMBER_OF_ENTITIES];
  struct tceu_app* plan;
};

struct tceu_app* create_empty_plan() {
  struct tceu_app* answer = malloc(sizeof(tceu_app));
  answer->data = malloc(sizeof(CEU_Main));
  answer->init = &ceu_app_init;
  answer->init(answer);
  return answer;
}

void pickup(struct planning_state* state, int parcel, int truck) {
  int name = state->gensym + 1;
  // PICKUP OPERATOR
  // pickup <parcel> with <truck>
  // after <prereqs>, and name this operation <name>
  // preconditions
  assert(state->pos[truck] == state->pos[parcel]);
  // effects
  {
    tceu__int__int__int payload = { name, parcel, truck };
    ceu_sys_go(state->plan, CEU_IN_PICKUP, &payload);
  }
  if (state->now[truck] > 0) {
    tceu__int__int payload = { state->now[truck], name };
    ceu_sys_go(state->plan, CEU_IN_BEFORE, &payload);
  }
  ceu_sys_go(state->plan, CEU_IN_DONE_CONFIG, &name);
  state->now[truck] = name;
  state->pos[parcel] = truck;
  state->gensym = name;
}

void dropoff(struct planning_state* state, int parcel, int truck, int location) {
  int name = state->gensym + 1;

  // DROPOFF OPERATOR
  // dropoff <parcel> (which is currently in <truck>) at <location>
  // after <prereqs>, and name this operation <name>
  // preconditions
  assert(state->pos[parcel] == truck);
  assert(state->pos[truck] == location);
  // effects
  tceu__int__int__int__int payload = { name, parcel, truck, location };
  ceu_sys_go(state->plan, CEU_IN_DROPOFF, &payload);
  if (state->now[truck] > 0) {
    tceu__int__int payload = { state->now[truck], name };
    ceu_sys_go(state->plan, CEU_IN_BEFORE, &payload);
  }
  ceu_sys_go(state->plan, CEU_IN_DONE_CONFIG, &name);
  state->now[truck] = name;
  state->pos[parcel] = location;
  state->gensym = name;
}

void move(struct planning_state* state, int truck, int from, int to) {
  int name = state->gensym + 1;

  // MOVE OPERATOR
  // move <truck> from <from> to <to>,
  // after <prereqs>, and name this operation <name>
  // preconditions
  assert(state->pos[truck] == from);
  assert(state->route[to] == from || state->route[from] == to);
  // effects
  tceu__int__int__int__int payload = { name, truck, from, to };
  ceu_sys_go(state->plan, CEU_IN_MOVE, &payload);
  if (state->now[truck] > 0) {
    tceu__int__int payload = { state->now[truck], name };
    ceu_sys_go(state->plan, CEU_IN_BEFORE, &payload);
  }
  ceu_sys_go(state->plan, CEU_IN_DONE_CONFIG, &name);

  state->now[truck] = name;
  state->pos[truck] = to;
  state->gensym = name;
}

// BUG: this returns inefficient routes
void route(struct planning_state* state, int truck, int location) {
  int down[NUMBER_OF_LOCATIONS] = {};
  int i, j, k, l;
  int destination_cursor = location;
  for (j = NUMBER_OF_LOCATIONS - 1; j >= 0; j--) {
    int previous = state->route[destination_cursor];
    if (previous == ROOT) {
      break;
    }
    down[j] = previous;
    destination_cursor = previous;
  }
  while (state->pos[truck] != location) { 
    for (k = j; k < NUMBER_OF_LOCATIONS; k++) {
      if (state->pos[truck] == down[k]) {
        for (l = k+1; l < NUMBER_OF_LOCATIONS; l++) {
          move(state, truck, state->pos[truck], down[l]);
        }
        move(state, truck, state->pos[truck], location);
        return;
      }
    }
    assert(state->route[state->pos[truck]] != ROOT);
    move(state, truck, state->pos[truck], state->route[state->pos[truck]]);
  }
}

void move_parcels(struct planning_state* state, struct goal* goal) {
  int truck_index = 0;
  int parcel;

  for (parcel = 0; parcel < NUMBER_OF_ENTITIES; parcel++) {
    int destination = goal->pos[parcel];
    if (destination == DONT_CARE) {
      continue;
    }
    if (state->pos[parcel] != destination) {
      int truck = state->trucks[truck_index];
      route(state, truck, state->pos[parcel]);
      pickup(state, parcel, truck);
      route(state, truck, destination);
      dropoff(state, parcel, truck, destination);
      truck_index++;
      if (truck_index >= state->number_of_trucks) {
        truck_index = 0;
      }
    }
  }
}

struct tceu_app* chop(struct model* model, struct goal* goal) {
  struct planning_state planning_state = {};
  int i;
  for (i = 0; i < NUMBER_OF_LOCATIONS; i++) {
    planning_state.route[i] = model->route[i];
  }
  for (i = 0; i < NUMBER_OF_ENTITIES; i++) {
    planning_state.pos[i] = model->pos[i];
  }

  planning_state.trucks = model->trucks;
  planning_state.number_of_trucks = model->number_of_trucks;
  planning_state.gensym = 100;
  planning_state.plan = create_empty_plan();

  move_parcels(&planning_state, goal);
  return planning_state.plan;
}

