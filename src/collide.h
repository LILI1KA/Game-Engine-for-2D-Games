#ifndef COLLIDE_H
#define COLLIDE_H

#include "collide_state.h"

class Collide {
    CollideState c_state;
  public:
    Collide();
    CollideState getState();
    void setState(CollideState new_state);
};


#endif
