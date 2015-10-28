#ifndef CHOP_H
#define CHOP_H

// locations
enum {
  NW,
  N,
  NE,
  W,
  C,
  E,
  SW,
  S,
  SE,

  NUMBER_OF_LOCATIONS,

  // things that are like a location,
  // but are not actually locations.
  ROOT,
  DONT_CARE,
};

// entities
enum {
  P1,
  P2,
  P3,
  P4,
  P5,
  P6,
  P7,
  P8,
  P9,
  T1,
  T2,
  T3,
  
  NUMBER_OF_ENTITIES
};

struct model {
  int route[NUMBER_OF_LOCATIONS];
  int pos[NUMBER_OF_ENTITIES];
  int *trucks;
  int number_of_trucks;
};

struct goal {
  int pos[NUMBER_OF_ENTITIES];
};

struct tceu_app* chop(struct model*, struct goal*);



#endif
