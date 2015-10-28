#include "chop.h"

#include <assert.h>
#include <stdio.h>
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

// OPERATORS

// pickup <parcel> with <truck>
// after <prereqs>, and name this operation <name>
void pickup_operator(struct planning_state* state, int name, int prereqs[], int parcel, int truck)
{
  // preconditions
  // assert(state->pos[truck] == state->pos[parcel])
  // effects
  state->gensym = name;
  state->now[parcel] = name;
  state->now[truck] = name;
  state->pos[parcel] = truck;
  tceu__int__int__int payload = { name, parcel, truck };
  ceu_sys_go(state->plan, CEU_IN_PICKUP, &payload);
}

// dropoff <parcel> (which is currently in <truck>) at <location>
// after <prereqs>, and name this operation <name>
void dropoff_operator(struct planning_state* state, int name, int prereqs[], int parcel, int truck, int location)
{
  // preconditions
  assert(state->pos[parcel] == truck);
  // assert(state->pos[truck] == location);
  // effects
  state->gensym = name;
  state->now[parcel] = name;
  state->now[truck] = name;
  state->pos[parcel] = location;
  tceu__int__int__int__int payload = { name, parcel, truck, location };
  ceu_sys_go(state->plan, CEU_IN_DROPOFF, &payload);
}

// METHODS

void pickup_method(struct planning_state* state, int parcel, int truck) {
  int name = state->gensym + 1;
  int prereqs[2] = {};
  prereqs[0] = state->now[parcel];
  int truck_now = state->now[truck];
  if (truck_now != prereqs[0]) {
    prereqs[1] = truck_now;
  }
  pickup_operator(state, name, prereqs, parcel, truck);
}

void dropoff_method(struct planning_state* state, int parcel, int truck, int destination) {
  int name = state->gensym + 1;
  int prereqs[2] = {};
  prereqs[0] = state->now[parcel];
  int truck_now = state->now[truck];
  if (truck_now != prereqs[0]) {
    prereqs[1] = truck_now;
  }
  dropoff_operator(state, name, prereqs, parcel, truck, destination);
}

void move_parcels(struct planning_state* state, struct goal* goal) {
  int truck = state->trucks[0];
  int parcel;

  for (parcel = 0; parcel < NUMBER_OF_ENTITIES; parcel++) {
    int destination = goal->pos[parcel];
    if (destination == DONT_CARE) {
      continue;
    }
    if (state->pos[parcel] != destination) {
      // TODO: route truck from wherever it is to whereever the parcel is
      pickup_method(state, parcel, truck);
      // TODO: route truck from wherever it is to wherever the destination is
      dropoff_method(state, parcel, truck, destination);
      truck++;
      if (truck >= state->number_of_trucks) {
        truck = 0;
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
  planning_state.plan = create_empty_plan();

  move_parcels(&planning_state, goal);
  return planning_state.plan;
}

