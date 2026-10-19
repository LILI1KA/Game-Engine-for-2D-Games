#ifndef ARLG_CLIENT_H
#define ARLG_CLIENT_H

#include <iostream>
#include <memory>

#include "model.h"
#include "cursekeyboard.h"


class Player: public SingleChar {
    int health;
  public:
    Player();
    int getHealth();
    void setHealth(int new_health);
    void collideWith(std::shared_ptr<Object> object,std::shared_ptr<Collide> collide) override;
};


class Fire: public SingleChar {
  public:
    Fire();
    void collideWith(std::shared_ptr<Object> object,std::shared_ptr<Collide> collide) override;
};

class Bullet: public SingleChar {
  public:
    Bullet();
    Bullet(char dir);
    void setDir(char new_dir);
    void collideWith(std::shared_ptr<Object> object,std::shared_ptr<Collide> collide) override;
};

class HealthPickUp: public SingleChar {
  public:
    HealthPickUp();
    void collideWith(std::shared_ptr<Object> object,std::shared_ptr<Collide> collide) override;
};

class Exit: public SingleChar {
  public:
    Exit();
};

class Walker: public Rectangle {
    int health;
  public:
    Walker();
    int getHealth();
    void setHealth(int new_health);
    void collideWith(std::shared_ptr<Object> object,std::shared_ptr<Collide> collide) override;
};

class Boss: public Bitmap {
    int health1;
    int health2;
    bool rage;
  public:
    Boss();
    int getHealth();
    void setHealth(int new_health);
    bool getRage();
    void setRage(bool new_rage);
    void change_state();
    void collideWith(std::shared_ptr<Object> object,std::shared_ptr<Collide> collide) override;
};


class Client {
    int level;
    std::shared_ptr<Model> model;
    std::shared_ptr<Player> player;
    std::shared_ptr<Exit> exit;
    std::shared_ptr<Boss> boss;
    std::vector<std::shared_ptr<Bullet>> bullets;
    std::vector<std::shared_ptr<Fire>> fires;
    std::vector<std::shared_ptr<HealthPickUp>> healthpicks;
    std::vector<std::shared_ptr<Walker>> walkers;
    bool start;
    int walker_cycle;
  public:
    Client();

    std::shared_ptr<Model> getModel();
    void setModel(std::shared_ptr<Model> new_model);
    std::shared_ptr<Player> getPlayer();
    void setPlayer(std::shared_ptr<Player> new_player);
    std::shared_ptr<Exit> getExit();
    void setExit(std::shared_ptr<Exit> new_exit);
    std::shared_ptr<Boss> getBoss();
    void setBoss(std::shared_ptr<Boss> new_boss);
    std::vector<std::shared_ptr<Bullet>> &getBullets();
    void setBullets(std::vector<std::shared_ptr<Bullet>> &new_bullets);
    std::vector<std::shared_ptr<Fire>> &getFires();
    void setFires(std::vector<std::shared_ptr<Fire>> &new_fires);
    std::vector<std::shared_ptr<HealthPickUp>> &getHealthPickUps();
    void setHealthPickUps(std::vector<std::shared_ptr<HealthPickUp>> &new_healthpicks);


    void walker_function(Subject*);

    void add(std::shared_ptr<Bullet> bullet);
    void add(std::shared_ptr<Fire> fire);
    void add(std::shared_ptr<HealthPickUp> healthpick);
    void add(std::shared_ptr<Walker> walker);

    void LuckyPick();
    void update_inner();

    //void addBullet(char dir);

    int getLevel();
    void setLevel(int new_level);

    void init();
    void clearLevel();
    void initLevel1();
    void initLevel2();
    void initLevel3();
    void update();
    void add_all();
    bool win();

    void go();

};


#endif
