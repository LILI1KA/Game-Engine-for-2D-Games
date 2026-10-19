#include <map>
#include <memory>
#include "subject.h"
#include "object.h"


#include <iostream>

Object::Object(): collide_map{std::map<std::shared_ptr<Object>,std::shared_ptr<Collide>>()},
                  x{int()}, y{int()}, height{int()}, prev_x{-1}, prev_y{-1}, prev_height{-1}, max_out{10}, out_count{0}, 
                  c_state{CollideStateType::Pass}, action{ACTION::NONE} {}

Object::Object(int new_x, int new_y): Object() {
    x = new_x;
    y = new_y;
}

int Object::getX() { return x; }

int Object::getY() { return y; }

int Object::getHeight() { return height; }
    
void Object::setX(int new_x) { x = new_x; }

void Object::setY(int new_y) { y = new_y; }

void Object::setHeight(int new_height) { height = new_height; }

int Object::getPrevx() { return prev_x; }

int Object::getPrevy() { return prev_y; }

int Object::getPrevheight() { return prev_height; }

int Object::getOutCount() { return out_count; }

int Object::getMaxCount() { return max_out; }

void Object::updateOutCount() { ++out_count; }

void Object::resetOutCount() { out_count = 0; }

void Object::add(std::shared_ptr<Object> object,std::shared_ptr<Collide> collide) { collide_map[object] = collide; }

void Object::add(std::shared_ptr<Movement> movement) { attach(movement); }

CollideState Object::getState() { return c_state; }

void Object::setState(CollideState new_state) { c_state = new_state; }

void Object::record() {
    prev_x = x;
    prev_y = y;
    prev_height = height;
}

void Object::move() {
    record();
    notifyObservers();
}

void Object::collideWith(std::shared_ptr<Object> object,std::shared_ptr<Collide> collide) {
    CollideStateType c_state_type = collide->getState().type;
    if (!is_collision(object)) return;
    switch (c_state_type) {
        case CollideStateType::Pass: {
            break;
        }
        case CollideStateType::Bounce: {
            if (getState().type != CollideStateType::Win && getState().type != CollideStateType::Lose 
                && getState().type != CollideStateType::Damage && getState().type != CollideStateType::Stop) {
                    setState(CollideState{CollideStateType::Bounce});
                    if (bounce_h(object)) {
                        invertAll_h();
                    }
                    if (bounce_v(object)) {
                        invertAll_v();
                    }
            }
            break;
        }
        case CollideStateType::Stop: {
            if (getState().type != CollideStateType::Win && getState().type != CollideStateType::Lose 
                && getState().type != CollideStateType::Damage) 
                setState(CollideState{CollideStateType::Stop});
            detachAll();
            break;
        }
        case CollideStateType::Damage: {
            if (getState().type != CollideStateType::Win && getState().type != CollideStateType::Lose) 
                setState(CollideStateType::Damage);
            break;
        }
        case CollideStateType::Win: {
            setState(CollideState{CollideStateType::Win});
            break;
        }
        case CollideStateType::Lose: {
            setState(CollideState{CollideStateType::Lose});
            break;
        }
        default: {
            break;
        }
    }
}

void Object::collideAll() { for (auto &it:collide_map) collideWith(it.first, it.second); }

void Object::reset() {
    x = prev_x;
    y = prev_y;
    height = prev_height;
}

ACTION Object::getAction() { return action; }

void Object::setAction(ACTION new_action) { action = new_action; }

void Object::del(std::vector<std::shared_ptr<Subject>> &vos) {
    for (auto &subject:vos) {
        std::shared_ptr<Object> obj = std::dynamic_pointer_cast<Object> (subject);
        collide_map.erase(obj);
    }
}

void Object::clear_map() { collide_map.clear(); }


SingleChar::SingleChar(): Object(), c{char()} {}

SingleChar::SingleChar(int x, int y, char c): Object{x, y}, c{c} {}

char SingleChar::getChar() { return c; }

void SingleChar::setChar(char new_c) { c = new_c; }

void SingleChar::move() {
    int x = getX(), y = getY();
    if ( !((x > 0) && (x < 21) && (y > 0) && (y < 79)) ) {
        updateOutCount();
    } else {
        resetOutCount();
    }
    if (getOutCount() == getMaxCount()) {
        setState(CollideStateType::Damage);
        return;
    }
    record();
    notifyObservers();
}

void SingleChar::move(StraightLine &straight_line) {
    int vx = straight_line.getVx();
    int vy = straight_line.getVy();
    int vh = straight_line.getVh();
    setX(getX() + vx);
    setY(getY() + vy);
    setHeight(getHeight() + vh);
}

void SingleChar::move(Gravitate &gravitate) {
    char direction = gravitate.getDir();
    switch (direction) {
        case 'u': {
            setX(getX() - 1);
            break;
        }
        case 'd': {
            setX(getX() + 1);
            break;
        }
        case 'l': {
            setY(getY() - 1);
            break;
        }
        case 'r': {
            setY(getY() + 1);
            break;
        }
        default: {
            break;
        }
    }
}

void SingleChar::move(Cycle &cycle) {
    auto function = cycle.getFunc();
    function(this);
}


bool SingleChar::is_collision(std::shared_ptr<Object> object) {
    Object *o_ptr = &(*object);
    SingleChar *s_ptr = dynamic_cast<SingleChar*> (o_ptr);
    Rectangle *r_ptr = dynamic_cast<Rectangle*> (o_ptr);
    Bitmap *b_ptr = dynamic_cast<Bitmap*> (o_ptr);
    if (s_ptr) {
        return (getHeight() == s_ptr->getHeight() && getX() == s_ptr->getX() && getY() == s_ptr->getY());
    } else if (r_ptr) {
        return (getHeight() == r_ptr->getHeight() && 
                !(getX() < r_ptr->getX()) && !(getX() > r_ptr->getX()+r_ptr->getWidth()-1) &&
                !(getY() < r_ptr->getY()) && !(getY() > r_ptr->getY()+r_ptr->getLength()-1));
    } else if (b_ptr) {
        for (size_t i = 0; i < b_ptr->size(); ++i) {
            if (is_collision(b_ptr->at_index(i))) return true;
        }
        return false;
    }
    return false;
}

bool SingleChar::bounce_h(std::shared_ptr<Object> other) {
    Object *o_ptr = &(*other);
    SingleChar *s_ptr = dynamic_cast<SingleChar*> (o_ptr);
    Rectangle *r_ptr = dynamic_cast<Rectangle*> (o_ptr);
    Bitmap *b_ptr = dynamic_cast<Bitmap*> (o_ptr);
    int x1 = getX(), y1 = getY();
    if (s_ptr) {
        return ( !(s_ptr->getPrevy() > y1) && !(getPrevy() < y1) )
            || ( !(s_ptr->getPrevy() < y1) && !(getPrevy() > y1) );
    } else if (r_ptr) {
        int x2 = r_ptr->getX(), y2 = r_ptr->getY(), width2 = r_ptr->getWidth(), length2 = r_ptr->getLength();
        return ((y1 == y2) && !(getPrevy() > y1)) || ((y1 == y2+length2-1) && !(getPrevy() < y1));
    } else if (b_ptr) {
        for (size_t i = 0; i < b_ptr->size(); ++i) {
            if (!is_collision(b_ptr->at_index(i))) continue;
            if (!bounce_h(b_ptr->at_index(i))) return false;
        }
        true;
    }
    return false;
}

bool SingleChar::bounce_v(std::shared_ptr<Object> other) {
    Object *o_ptr = &(*other);
    SingleChar *s_ptr = dynamic_cast<SingleChar*> (o_ptr);
    Rectangle *r_ptr = dynamic_cast<Rectangle*> (o_ptr);
    Bitmap *b_ptr = dynamic_cast<Bitmap*> (o_ptr);
    int x1 = getX(), y1 = getY();
    if (s_ptr) {
        return ( !(s_ptr->getPrevx() > x1) && !(getPrevx() < x1) )
            || ( !(s_ptr->getPrevx() < x1) && !(getPrevx () > x1) );
    } else if (r_ptr) {
        int x2 = r_ptr->getX(), y2 = r_ptr->getY(), width2 = r_ptr->getWidth(), length2 = r_ptr->getLength();
        return ((x1 == x2) && !(getPrevx() > x1)) || ((x1 == x2+width2-1) && !(getPrevx() < x1));
    } else if (b_ptr) {
        for (size_t i = 0; i < b_ptr->size(); ++i) {
            if (!is_collision(b_ptr->at_index(i))) continue;
            if (!bounce_v(b_ptr->at_index(i))) return false;
        }
        return true;
    }
    return false;
}

void SingleChar::reset_all() { reset(); }


Rectangle::Rectangle(): Object(), c{char()}, length{int()}, width{int()}, prev_length{-1}, prev_width{-1} {}

Rectangle::Rectangle(int x, int y, char c, int length, int width): 
Object{x, y}, c{c}, length{length}, width{width}, prev_length{-1}, prev_width{-1}  {}

char Rectangle::getChar() { return c; }

void Rectangle::setChar(char new_c) { c = new_c; }

int Rectangle::getLength() { return length; }

int Rectangle::getWidth() { return width; }

int Rectangle::getPrevLength() { return prev_length; }

int Rectangle::getPrevWidth() { return prev_width; }

void Rectangle::setLength(int new_length) { length = new_length; }

void Rectangle::setWidth(int new_width) { width = new_width; }

void Rectangle::record_size() {
    prev_length = length;
    prev_width = width;
}

void Rectangle::move() {
    int x = getX(), y = getY(), width = getWidth(), length = getLength();
    if ( !((x > 0) && (x+width-1 < 21) && (y > 0) && (y+length-1 < 79)) ) {
        updateOutCount();
    } else {
        resetOutCount();
    }
    if (getOutCount() == getMaxCount()) {
        setState(CollideStateType::Damage);
        return;
    }
    record();
    record_size();
    notifyObservers();
}

void Rectangle::move(StraightLine &straight_line) {
    int vx = straight_line.getVx();
    int vy = straight_line.getVy();
    int vh = straight_line.getVh();
    setX(getX() + vx);
    setY(getY() + vy);
    setHeight(getHeight() + vh);
}

void Rectangle::move(Gravitate &gravitate) {
    char direction = gravitate.getDir();
    switch (direction) {
        case 'u': {
            setX(getX() - 1);
            break;
        }
        case 'd': {
            setX(getX() + 1);
            break;
        }
        case 'l': {
            setY(getY() - 1);
            break;
        }
        case 'r': {
            setY(getY() + 1);
            break;
        }
        default: {
            break;
        }
    }
}

void Rectangle::move(Cycle &cycle) {
    auto function = cycle.getFunc();
    function(this);
}


bool Rectangle::is_collision(std::shared_ptr<Object> object) {
    Object *o_ptr = &(*object);
    SingleChar *s_ptr = dynamic_cast<SingleChar*> (o_ptr);
    Rectangle *r_ptr = dynamic_cast<Rectangle*> (o_ptr);
    Bitmap *b_ptr = dynamic_cast<Bitmap*> (o_ptr);
    if (s_ptr) {
        return (s_ptr->getHeight() == getHeight() && 
                !(s_ptr->getX() < getX()) && !(s_ptr->getX() > getX()+getWidth()-1) &&
                !(s_ptr->getY() < getY()) && !(s_ptr->getY() > getY()+getLength()-1));
    } else if (r_ptr) {
        int x1 = getX(), y1 = getY(), wid1 = getWidth(), len1 = getLength(),
            x2 = r_ptr->getX(), y2 = r_ptr->getY(), wid2 = r_ptr->getWidth(), len2 = r_ptr->getLength();
        return (getHeight() == r_ptr->getHeight() &&
                !(((x1 < x2)? x2:x1) > ((x1+wid1-1 < x2+wid2-1)? x1+wid1-1:x2+wid2-1)) &&
                !(((y1 < y2)? y2:y1) > ((y1+len1-1 < y2+len2-1)? y1+len1-1:y2+len2-1)));
    } else if (b_ptr) {
        for (size_t i = 0; i < b_ptr->size(); ++i) {
            if (is_collision(b_ptr->at_index(i))) return true;
        }
        return false;
    }
    return false;
}

bool Rectangle::bounce_h(std::shared_ptr<Object> other) {
    Object *o_ptr = &(*other);
    SingleChar *s_ptr = dynamic_cast<SingleChar*> (o_ptr);
    Rectangle *r_ptr = dynamic_cast<Rectangle*> (o_ptr);
    Bitmap *b_ptr = dynamic_cast<Bitmap*> (o_ptr);
    int x1 = getX(), width1 = getWidth(), y1 = getY(), length1 = getLength();
    if (s_ptr) {
        int x2 = s_ptr->getX(), y2 = s_ptr->getY();
        return ((y1 == y2) && !(getPrevy() < y1)) || ((y1+length1-1 == y2) && !(getPrevy()+getPrevLength()-1 > y1+length1-1));
    } else if (r_ptr) {
        int x2 = r_ptr->getX(), y2 = r_ptr->getY(), width2 = r_ptr->getWidth(), length2 = r_ptr->getLength();
        return ((y1+length1-1 == y2) && !(getPrevy()+getPrevLength()-1 > y1+length1-1)) 
            || ((y1 == y2+length2-1) && !(getPrevy() < y1) );
    } else if (b_ptr) {
        for (size_t i = 0; i < b_ptr->size(); ++i) {
            if (!is_collision(b_ptr->at_index(i))) continue;
            if (bounce_h(b_ptr->at_index(i))) return true;
        }
    }
    return false;
}

bool Rectangle::bounce_v(std::shared_ptr<Object> other) {
    Object *o_ptr = &(*other);
    SingleChar *s_ptr = dynamic_cast<SingleChar*> (o_ptr);
    Rectangle *r_ptr = dynamic_cast<Rectangle*> (o_ptr);
    Bitmap *b_ptr = dynamic_cast<Bitmap*> (o_ptr);
    int x1 = getX(), width1 = getWidth(), y1 = getY(), length1 = getLength();
    if (s_ptr) {
        int x2 = s_ptr->getX(), y2 = s_ptr->getY();
        return ((x1 == x2) && !(getPrevx() < x1)) || ((x1+width1-1 == x2) && !(getPrevx()+getPrevWidth()-1 > x1+width1-1));
    } else if (r_ptr) {
        int x2 = r_ptr->getX(), y2 = r_ptr->getY(), width2 = r_ptr->getWidth(), length2 = r_ptr->getLength();
        return ((x1+width1-1 == x2) && !(getPrevx()+getPrevWidth()-1 > x1+width1-1)) 
            || ((x1 == x2+width2-1) && !(getPrevx() < x1));
    } else if (b_ptr) {
        for (size_t i = 0; i < b_ptr->size(); ++i) {
            if (!is_collision(b_ptr->at_index(i))) continue;
            if (bounce_v(b_ptr->at_index(i))) return true;
        }
    }
    return false;
}

void Rectangle::reset_all() {
    reset();
    width = prev_width;
    length = prev_length;
}



Bitmap::Bitmap(): Object(), chars{std::vector<std::shared_ptr<SingleChar>> ()} {}

std::shared_ptr<SingleChar> Bitmap::at_index(int i) { return chars.at(i); }

int Bitmap::size() { return chars.size(); }

void Bitmap::add_char(std::shared_ptr<SingleChar> new_char) { chars.emplace_back(new_char); }

void Bitmap::setHeights(int height) { for (auto &sc:chars) sc->setHeight(height); }

void Bitmap::record() { for (auto &sc:chars) sc->record(); }

void Bitmap::move() {
    int x = 0, y = 0;
    bool out_of_range = false;
    for (size_t i = 0; i < size(); ++i) {
        x = at_index(i)->getX(), y = at_index(i)->getY();
        if ( !((x > 0) && (x < 21) && (y > 0) && (y < 79)) ) {
            out_of_range = true;
            break;
        }
    }
    if (out_of_range) {
        updateOutCount();
    } else {
        resetOutCount();
    }
    if (getOutCount() == getMaxCount()) {
        setState(CollideStateType::Damage);
        return;
    }
    record();
    for (size_t i = 0; i < size(); ++i) at_index(i)->record();
    notifyObservers();
}

void Bitmap::move(StraightLine &straight_line) {  for (auto &sc:chars) sc->move(straight_line); }

void Bitmap::move(Gravitate &gravitate) {  for (auto &sc:chars) sc->move(gravitate); }

void Bitmap::move(Cycle &cycle) {
    auto function = cycle.getFunc();
    function(this);
}


bool Bitmap::is_collision(std::shared_ptr<Object> object) {
    Object *o_ptr = &(*object);
    for (size_t i = 0; i < size(); ++i) {
        if ((at_index(i))->is_collision(object)) return true;
    }
    return false;
}

bool Bitmap::bounce_h(std::shared_ptr<Object> other) {
    for (size_t i = 0; i < size(); ++i) {
        if (!is_collision(at_index(i))) continue;
        if (at_index(i)->bounce_h(other)) return true;
    }
    return false;
}

bool Bitmap::bounce_v(std::shared_ptr<Object> other) {
    for (size_t i = 0; i < size(); ++i) {
        if (!is_collision(at_index(i))) continue;
        if (at_index(i)->bounce_v(other)) return true;
    }
    return false;
}

void Bitmap::reset_all() {
    reset();
    for (size_t i = 0; i < size(); ++i) {
        at_index(i)->reset_all();
    }
}

void Bitmap::clear_all() { chars.clear(); }

