#include "collide.h"

Collide::Collide(): c_state{CollideStateType::Pass} {}
    
CollideState Collide::getState() { return c_state; }

void Collide::setState(CollideState new_state) { c_state = new_state; }

