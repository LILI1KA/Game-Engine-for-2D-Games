#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <memory>
#include "subject.h"
#include "observer.h"
#include "collide.h"
#include "end_state.h"
#include "movement.h"
#include "action.h"

class Object: public Subject {
    std::map<std::shared_ptr<Object>,std::shared_ptr<Collide>> collide_map;
    int x;
    int y;
    int height;
    int prev_x;
    int prev_y;
    int prev_height;
    int max_out;
    int out_count;
    CollideState c_state;
    ACTION action;
  public:
    Object();
    Object(int new_x, int new_y);
    int getX();
    int getY();
    int getHeight();
    void setX(int new_x);
    void setY(int new_y);
    void setHeight(int new_height);
    int getPrevx();
    int getPrevy();
    int getPrevheight();
    int getOutCount();
    int getMaxCount();
    void updateOutCount();
    void resetOutCount();
    void add(std::shared_ptr<Object> object,std::shared_ptr<Collide> collide);
    void add(std::shared_ptr<Movement> movement);
    CollideState getState();
    void setState(CollideState);
    virtual void record();
    virtual void move();
    virtual void move(StraightLine &straight_line) = 0; // same for both SingleChar and Rectangle, defined in superclass
    virtual void move(Gravitate &gravitate) = 0; // same for both SingleChar and Rectangle, defined in superclass
    virtual void move(Cycle &cycle) = 0;
    //virtual void move(PlayerControlled &player_controlled) = 0;
    virtual bool is_collision(std::shared_ptr<Object> object) = 0;
    virtual void collideWith(std::shared_ptr<Object> object,std::shared_ptr<Collide> collide);
    void collideAll();
    virtual bool bounce_h(std::shared_ptr<Object> other) = 0;
    virtual bool bounce_v(std::shared_ptr<Object> other) = 0;
    virtual void reset_all() = 0;
    void reset();
    ACTION getAction();
    void setAction(ACTION new_action);
    void del(std::vector<std::shared_ptr<Subject>> &vos);
    void clear_map();
};


class SingleChar: public Object {
    char c;
  public:
    SingleChar();
    SingleChar(int x, int y, char c);
    char getChar();
    void setChar(char new_char);
    void move() override;
    void move(StraightLine &straight_line) override;
    void move(Gravitate &gravitate) override;
    void move(Cycle &cycle) override;
    //void move(PlayerControlled &player_controlled) override;
    bool is_collision(std::shared_ptr<Object> object) override;
    bool bounce_h(std::shared_ptr<Object> other) override;
    bool bounce_v(std::shared_ptr<Object> other) override;
    void reset_all() override;
};

class Rectangle: public Object {
    char c;
    int length;
    int width;
    int prev_length;
    int prev_width;
  public:
    Rectangle();
    Rectangle(int x, int y, char c, int length, int width);
    char getChar();
    void setChar(char new_char);
    int getLength();
    int getWidth();
    int getPrevLength();
    int getPrevWidth();
    void setLength(int new_length);
    void setWidth(int new_width);
    void record_size();
    void move() override;
    void move(StraightLine &straight_line) override;
    void move(Gravitate &gravitate) override;
    void move(Cycle &cycle) override;
    //void move(PlayerControlled &player_controlled) override;
    bool is_collision(std::shared_ptr<Object> object) override;
    bool bounce_h(std::shared_ptr<Object> other) override;
    bool bounce_v(std::shared_ptr<Object> other) override;
    void reset_all() override;
};

class Bitmap: public Object {
    std::vector<std::shared_ptr<SingleChar>> chars;
  public:
    Bitmap();
    std::shared_ptr<SingleChar> at_index(int i);
    int size();
    void add_char(std::shared_ptr<SingleChar> new_char);
    void setHeights(int height);
    void record() override;
    void move() override;
    void move(StraightLine &straight_line) override;
    void move(Gravitate &gravitate) override;
    void move(Cycle &cycle) override;
    //void move(PlayerControlled &player_controlled) override;
    bool is_collision(std::shared_ptr<Object> object) override;
    bool bounce_h(std::shared_ptr<Object> other) override;
    bool bounce_v(std::shared_ptr<Object> other) override;
    void reset_all() override;
    void clear_all();
};





#endif
