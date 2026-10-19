#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <memory>
#include "view.h"
#include "controller.h"
#include "subject.h"
#include "object.h"
#include "end_state.h"

class Clock;

class Model {
    EndState e_state;
    std::shared_ptr<View> view;
    std::shared_ptr<Controller> controller;
    std::shared_ptr<Clock> clock;
    std::vector<std::shared_ptr<Subject>> objects;
    std::vector<std::shared_ptr<Subject>> will_be_deleted;
  public:
    Model();
    void init();
    void display();
    void update();
    void border_collision(std::shared_ptr<Object> obj);
    void set_status(std::string &str1, std::string &str2, std::string &str3);
    void add(std::shared_ptr<Subject> new_obj);
    std::shared_ptr<View> get_view();
    std::shared_ptr<Controller> get_controller();
    std::shared_ptr<Status> get_status();
    std::shared_ptr<Border> get_border();
    std::shared_ptr<Subject> at(int i);
    void detach(std::shared_ptr<Subject> subject);
    void detach_all();
    EndState get_state();
    void set_state(EndStateType type);
    void set_win();
    void set_lose();
    void set_solid();
    std::string state_message();
    void tick();
    void move_all();
    void collide_all();

    int size() {return objects.size();}
};


#endif
