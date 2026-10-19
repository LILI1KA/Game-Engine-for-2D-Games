#ifndef MOVEMENT_H
#define MOVEMENT_H
#include "observer.h"
#include "controller.h"

class Movement: public Observer {};

class StraightLine: public Movement {
    int vx;
    int vy;
    int vh;
  public:
    StraightLine();
    StraightLine(int vx, int vy);
    StraightLine(int vx, int vy, int vh);
    void notify(Subject &subject) override;
    int getVx();
    int getVy();
    int getVh();
    void setVx(int new_vx);
    void setVy(int new_vy);
    void setVh(int new_vh);
    void invert_h() override;
    void invert_v() override;
};

class Cycle: public Movement {
    typedef void (*cycle_function)(Subject*);
    cycle_function function;
    int current_cycle;
    int max_cycle;
  public:
    Cycle();
    Cycle(cycle_function function, int max_cycle);
    void notify(Subject &subject) override;
    cycle_function getFunc();
    void setFunc(cycle_function new_function);
};

class Gravitate: public Movement {
    char direction; //up(u),down(d),left(l),right(r)
  public:
    Gravitate();
    Gravitate(char direction);
    void notify(Subject &subject) override;
    char getDir();
    void setDir(char new_direction);
    void invert_h() override;
    void invert_v() override;
};

class PlayerControlled: public Movement {
    std::shared_ptr<Controller> controller;
  public:
    PlayerControlled();
    PlayerControlled(std::shared_ptr<Controller> controller);
    std::shared_ptr<Controller> getController();
    void setController(std::shared_ptr<Controller> new_controller);
    void notify(Subject &subject) override;
};


#endif
