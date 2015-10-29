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
  // assert(state->pos[truck] == state->pos[parcel])
  // effects
  {
    tceu__int__int__int payload = { name, parcel, truck };
    ceu_sys_go(state->plan, CEU_IN_PICKUP, &payload);
  }
  if (state->now[parcel] > 0) {
    tceu__int__int payload = { state->now[parcel], name };
    ceu_sys_go(state->plan, CEU_IN_BEFORE, &payload);
  }
  if (state->now[truck] > 0 && state->now[truck] != state->now[parcel]) {
    tceu__int__int payload = { state->now[truck], name };
    ceu_sys_go(state->plan, CEU_IN_BEFORE, &payload);
  }
  ceu_sys_go(state->plan, CEU_IN_DONE_CONFIG, &name);
  state->gensym = name;
  state->now[parcel] = name;
  state->now[truck] = name;
  state->pos[parcel] = truck;
}

void dropoff(struct planning_state* state, int parcel, int truck, int location) {
  int name = state->gensym + 1;

  // DROPOFF OPERATOR
  // dropoff <parcel> (which is currently in <truck>) at <location>
  // after <prereqs>, and name this operation <name>
  // preconditions
  assert(state->pos[parcel] == truck);
  // assert(state->pos[truck] == location);
  // effects
  tceu__int__int__int__int payload = { name, parcel, truck, location };
  ceu_sys_go(state->plan, CEU_IN_DROPOFF, &payload);
  if (state->now[parcel] > 0) {
    tceu__int__int payload = { state->now[parcel], name };
    ceu_sys_go(state->plan, CEU_IN_BEFORE, &payload);
  }
  if (state->now[truck] > 0 && state->now[truck] != state->now[parcel]) {
    tceu__int__int payload = { state->now[truck], name };
    ceu_sys_go(state->plan, CEU_IN_BEFORE, &payload);
  }
  ceu_sys_go(state->plan, CEU_IN_DONE_CONFIG, &name);
  state->gensym = name;
  state->now[parcel] = name;
  state->now[truck] = name;
  state->pos[parcel] = location;
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
      // TODO: route truck from wherever it is to whereever the parcel is
      pickup(state, parcel, truck);
      // TODO: route truck from wherever it is to wherever the destination is
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
  memcpy(planning_state.route, model->route, sizeof(planning_state.route));
  memcpy(planning_state.pos, model->pos, sizeof(planning_state.pos));

  planning_state.trucks = model->trucks;
  planning_state.number_of_trucks = model->number_of_trucks;
  planning_state.gensym = 100;
  planning_state.plan = create_empty_plan();

  move_parcels(&planning_state, goal);
  return planning_state.plan;
}

