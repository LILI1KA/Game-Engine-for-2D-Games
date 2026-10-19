#include "movement.h"
#include "object.h"

#include <iostream>

StraightLine::StraightLine(): vx{int()}, vy{int()}, vh{int()} {}

StraightLine::StraightLine(int vx, int vy): vx{vx}, vy{vy}, vh{0} {}

StraightLine::StraightLine(int vx, int vy, int vh): vx{vx}, vy{vy}, vh{vh} {}

void StraightLine::notify(Subject &subject) {
    Object *o_ptr = dynamic_cast<Object*> (&subject);
    o_ptr->move(*this);
}
    
int StraightLine::getVx() { return vx; }

int StraightLine::getVy() { return vy; }

int StraightLine::getVh() { return vh; }

void StraightLine::setVx(int new_vx) { vx = new_vx; }

void StraightLine::setVy(int new_vy) { vy = new_vy; }

void StraightLine::setVh(int new_vh) { vy = new_vh; }

void StraightLine::invert_h() { 
    vy = -vy;
}

void StraightLine::invert_v() {
    vx = -vx;
}


Cycle::Cycle(): function{nullptr}, current_cycle{1}, max_cycle{1} {}

Cycle::Cycle(cycle_function function, int max_cycle): function{function}, current_cycle{1}, max_cycle{max_cycle} {}

void Cycle::notify(Subject &subject) {
    Object *o_ptr = dynamic_cast<Object*> (&subject);
    if (current_cycle == max_cycle) {
        o_ptr->move(*this);
        current_cycle = 1;
    } else {
        ++current_cycle;
    }
}

Cycle::cycle_function Cycle::getFunc() { return function; }

void Cycle::setFunc(cycle_function new_function) { function = new_function; }



Gravitate::Gravitate(): direction{'d'} {}

Gravitate::Gravitate(char direction): direction{direction} {}

void Gravitate::notify(Subject &subject) {
    Object *o_ptr = dynamic_cast<Object*> (&subject);
    o_ptr->move(*this);
}

char Gravitate::getDir() { return direction; }

void Gravitate::setDir(char new_direction) { direction = new_direction; }

void Gravitate::invert_h() { 
    if (direction == 'l') {
        direction == 'r';
    } else if (direction == 'r') {
        direction == 'l';
    }
}

void Gravitate::invert_v() { 
    if (direction == 'u') {
        direction == 'd';
    } else if (direction == 'd') {
        direction == 'u';
    }
}


PlayerControlled::PlayerControlled(): controller{nullptr} {}

PlayerControlled::PlayerControlled(std::shared_ptr<Controller> controller): controller{controller} {}

std::shared_ptr<Controller> PlayerControlled::getController() { return controller; }

void PlayerControlled::setController(std::shared_ptr<Controller> new_controller) { controller = new_controller; }

void PlayerControlled::notify(Subject &subject) { controller->controller_move(subject); }

