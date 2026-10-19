#ifndef BREAK_CLIENT_H
#define BREAK_CLIENT_H

#include <iostream>
#include <memory>

#include "model.h"
#include "cursekeyboard.h"

class Client {
    int level;
    std::shared_ptr<Model> model;
    std::shared_ptr<Bitmap> player;
    std::shared_ptr<Bitmap> wall;
    std::vector<std::shared_ptr<Rectangle>> bricks;
    std::shared_ptr<SingleChar> ball;
    bool start;
  public:
    Client();

    std::shared_ptr<Model> getModel();
    void setModel(std::shared_ptr<Model> new_model);
    std::shared_ptr<Bitmap> getPlayer();
    void setPlayer(std::shared_ptr<Bitmap> new_player);
    std::shared_ptr<Bitmap> getWall();
    void setWall(std::shared_ptr<Bitmap> new_wall);
    std::vector<std::shared_ptr<Rectangle>> &getBricks();
    void setBricks(std::vector<std::shared_ptr<Rectangle>> &new_bricks);
    std::shared_ptr<SingleChar> getBall();
    void setBall(std::shared_ptr<SingleChar> new_ball);


    void addWall(std::shared_ptr<SingleChar> wall);
    void addBrick(std::shared_ptr<Rectangle> brick);

    int getLevel();
    void setLevel(int new_level);

    void init();
    void clearLevel();
    void initLevel1();
    void initLevel2();
    void update();
    void add_all();
    bool win();

    void go();

};


#endif
