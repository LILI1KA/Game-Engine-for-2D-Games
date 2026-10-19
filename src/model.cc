#include "model.h"
#include <string>
#include <memory>
#include "view.h"
#include "controller.h"
#include "subject.h"
#include "object.h"
#include "clock.h"

#include <bits/stdc++.h>

#include <iostream>

Model::Model(): e_state{EndState()}, 
                view{std::make_shared<View>()}, controller{nullptr}, clock{std::make_shared<Clock>()},
                objects{std::vector<std::shared_ptr<Subject>>()}, will_be_deleted{std::vector<std::shared_ptr<Subject>>()} {}

void Model::init() { view->init(); }


bool compareObjects(std::shared_ptr<Subject> subj_1, std::shared_ptr<Subject> subj_2) {
    Object *obj_1 = dynamic_cast<Object*> (&(*subj_1));
    Object *obj_2 = dynamic_cast<Object*> (&(*subj_2));
    bool result = true;
    if (obj_1 && obj_2) if (obj_1->getHeight() < obj_2->getHeight()) result = false;
    return !result;
}



void Model::update() {
    if (get_state().type == EndStateType::Win || get_state().type == EndStateType::Lose) return;
    tick();
    view->erase();
    move_all();
    for (auto &ob:objects) {
        std::shared_ptr<Object> obj = std::dynamic_pointer_cast<Object> (ob);
        obj->del(will_be_deleted);
    }
    for (auto &del_obj:will_be_deleted) detach(del_obj);
    collide_all();
    will_be_deleted.clear();
    std::vector<std::shared_ptr<Subject>> will_be_reset;
    for (auto &ob:objects) {
        std::shared_ptr<Object> obj = std::dynamic_pointer_cast<Object> (ob);
        CollideStateType obj_state_type = (obj->getState()).type;
        if (obj_state_type == CollideStateType::Bounce) {
            will_be_reset.emplace_back(ob);
            obj->setState(CollideState{CollideStateType::Pass});
        } else if (obj_state_type == CollideStateType::Damage) {
            will_be_deleted.emplace_back(ob);
        } else if ((get_state().type != EndStateType::Lose && get_state().type != EndStateType::Win) &&
                    (obj_state_type == CollideStateType::Lose || obj_state_type == CollideStateType::Win)) {
            if (obj_state_type == CollideStateType::Lose) {
                set_lose();
            } else if (obj_state_type == CollideStateType::Win) {
                set_win();
            }
        }
    }
    for (auto &reset_sbj:will_be_reset) {
        Object *reset_obj = dynamic_cast<Object*> (&(*reset_sbj));
        if (reset_obj) reset_obj->reset_all();
    }
    if ((get_border()->get_state()).type == BorderStateType::Solid) {
        for (auto &ob:objects) {
            std::shared_ptr<Object> obj = std::dynamic_pointer_cast<Object> (ob);
            border_collision(obj);
        }
    }
}

void Model::display() {
    sort(objects.begin(), objects.end(), compareObjects);
    for (auto &ob:objects) view->add(ob);
    view->display();
}

void Model::border_collision(std::shared_ptr<Object> obj) {
    SingleChar *s_ptr = dynamic_cast<SingleChar*> (&(*obj));
    Rectangle *r_ptr = dynamic_cast<Rectangle*> (&(*obj));
    Bitmap *b_ptr = dynamic_cast<Bitmap*> (&(*obj));
    if (s_ptr) {
        int x = s_ptr->getX(), y = s_ptr->getY();
        if (0 < x && x < 21 && 0 < y && y < 79) return;
        s_ptr->reset_all();
        if (y == 0 || y == 79) s_ptr->invertAll_h();
        if (x == 0 || x == 21) s_ptr->invertAll_v();
    } else if (r_ptr) {
        int x = r_ptr->getX(), y = r_ptr->getY(), width = r_ptr->getWidth(), length = r_ptr->getLength();
        if (0 < x && x+width-1 < 21 && 0 < y && y+length-1 < 79) return;
        r_ptr->reset_all();
        if (y == 0 || y+length-1 == 79) r_ptr->invertAll_h();
        if (x == 0 || x+width-1 == 21) r_ptr->invertAll_v();
    } else if (b_ptr) {
        int x = 0, y = 0;
        bool touch_h = false, touch_v = false;
        for (size_t i = 0; i < b_ptr->size(); ++i) {
            x = b_ptr->at_index(i)->getX(), y = b_ptr->at_index(i)->getY();
            if (y == 0 || y == 79) touch_h = true;
            if (x == 0 || x == 21) touch_v = true;
            if (touch_h && touch_v) break;
        }
        if (!touch_h && !touch_v) return;
        b_ptr->reset_all();
        if (touch_h) b_ptr->invertAll_h();
        if (touch_v) b_ptr->invertAll_v();
    }
}

void Model::set_status(std::string &str1, std::string &str2, std::string &str3) { get_status()->set_line(str1, str2, str3); }

void Model::add(std::shared_ptr<Subject> new_obj) { objects.emplace_back(new_obj); }

std::shared_ptr<View> Model::get_view() { return view; }

// not yet
//std::shared_ptr<Controller> Model::get_controller();

std::shared_ptr<Status> Model::get_status() { return view->get_status(); }

std::shared_ptr<Border> Model::get_border() { return view->get_border(); }

std::shared_ptr<Subject> Model::at(int i) { return objects[i]; }

void Model::detach(std::shared_ptr<Subject> subject) {
    for (auto it = objects.begin(); it != objects.end(); ++it) {
        if (*it == subject) {
            objects.erase(it);
            break;
        }
    }
}

void Model::detach_all() { 
    objects.clear();
    will_be_deleted.clear();
}

EndState Model::get_state() { return e_state; }

void Model::set_state(EndStateType type) { e_state = EndState{type}; }

void Model::set_win() { set_state(EndStateType::Win); }
    
void Model::set_lose() { set_state(EndStateType::Lose); }

void Model::set_solid() { get_border()->set_state(BorderStateType::Solid); }
    
std::string Model::state_message() {
    switch (get_state().type) {
        case EndStateType::Win: {
            return "Win";
            break;
        } 
        case EndStateType::Lose: {
            return "Lose";
            break;
        }
        default: {
            return "Continue";
            break;
        }
    }
}

void Model::tick() { clock->tick(); }

void Model::move_all() { 
    for (auto &obj:objects) {
        Object *obj_ptr = dynamic_cast<Object*> (&(*obj));
        obj_ptr->record();
        obj_ptr->move();
        Rectangle *rec_ptr = dynamic_cast<Rectangle*> (obj_ptr);
        if (rec_ptr) rec_ptr->record_size();
    }
}

void Model::collide_all() { 
    for (auto &obj:objects) {
        Object *obj_ptr = dynamic_cast<Object*> (&(*obj));
        obj_ptr->collideAll();
    }
}

