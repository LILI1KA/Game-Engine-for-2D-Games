#ifndef COLLIDE_STATE_H
#define COLLIDE_STATE_H

enum class CollideStateType { Pass, Bounce, Stop, Damage, Win, Lose };

struct CollideState {
    CollideStateType type;
    CollideState() { type = CollideStateType::Pass; }
    CollideState( CollideStateType type ): type{type} {}
};


#endif
