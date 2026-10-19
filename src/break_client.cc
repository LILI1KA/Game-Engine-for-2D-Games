#include "break_client.h"

/*
class Client {
    int level;
    std::shared_ptr<Model> model;
    std::shared_ptr<Bitmap> player;
    std::shared_ptr<Bitmap> wall;
    std::vector<std::shared_ptr<SingleChar>> bricks;
    std::vector<SingleChar> ball;
  public:
    Client();

    std::shared_ptr<Model> getModel();
    void setModel(std::shared_ptr<Model> new_model);
    std::shared_ptr<Bitmap> getPlayer();
    void setPlayer(std::shared_ptr<Bitmap> new_player);
    std::shared_ptr<Bitmap> getWall();
    void setWall(std::shared_ptr<Bitmap> new_wall);
    std::vector<std::shared_ptr<SingleChar>> &getBricks();
    void setBricks(std::vector<std::shared_ptr<SingleChar>> &new_bricks);
    std::vector<SingleChar> getBall();
    void setBall(std::shared_ptr<SingleChar> new_ball);


    void addWall(std::shared_ptr<SingleChar> wall);
    void addBrick(std::shared_ptr<SingleChar> brick);

};
*/


Client::Client(): level{1}, model{std::make_shared<Model>()}, player{std::make_shared<Bitmap>()}, wall{std::make_shared<Bitmap>()},
                  bricks{std::vector<std::shared_ptr<Rectangle>>()}, ball{std::make_shared<SingleChar>()}, start{false} {}

std::shared_ptr<Model> Client::getModel() { return model; }

void Client::setModel(std::shared_ptr<Model> new_model) { model = new_model; }

std::shared_ptr<Bitmap> Client::getPlayer() { return player; }

void Client::setPlayer(std::shared_ptr<Bitmap> new_player) { player = new_player; }
    
std::shared_ptr<Bitmap> Client::getWall() { return wall; }

void Client::setWall(std::shared_ptr<Bitmap> new_wall) { wall = new_wall; }

std::vector<std::shared_ptr<Rectangle>> &Client::getBricks() { return bricks; }

void Client::setBricks(std::vector<std::shared_ptr<Rectangle>> &new_bricks) { bricks = new_bricks; }
    
std::shared_ptr<SingleChar> Client::getBall() { return ball; }

void Client::setBall(std::shared_ptr<SingleChar> new_ball) { ball = new_ball; }


void Client::addWall(std::shared_ptr<SingleChar> new_char) { wall->add_char(new_char); }

void Client::addBrick(std::shared_ptr<Rectangle> brick) { bricks.emplace_back(brick); }

int Client::getLevel() { return level; }

void Client::setLevel(int new_level) { level = new_level; }


void Client::init() {
  model->set_solid();
  player = std::make_shared<Bitmap>();
  ball = std::make_shared<SingleChar> (19,34,'o');
  wall = std::make_shared<Bitmap>();
  auto p1 = std::make_shared<SingleChar> (20,29,'-');
  auto p2 = std::make_shared<SingleChar> (20,30,'-');
  auto p3 = std::make_shared<SingleChar> (20,31,'-');
  auto p4 = std::make_shared<SingleChar> (20,32,'-');
  auto p5 = std::make_shared<SingleChar> (20,33,'-');
  auto p6 = std::make_shared<SingleChar> (20,34,'-');
  auto p7 = std::make_shared<SingleChar> (20,35,'-');
  auto p8 = std::make_shared<SingleChar> (20,36,'-');
  auto p9 = std::make_shared<SingleChar> (20,37,'-');
  auto p10 = std::make_shared<SingleChar> (20,38,'-');
  auto p11 = std::make_shared<SingleChar> (20,39,'-');
  player->add_char(p1);
  player->add_char(p2);
  player->add_char(p3);
  player->add_char(p4);
  player->add_char(p5);
  player->add_char(p6);
  player->add_char(p7);
  player->add_char(p8);
  player->add_char(p9);
  player->add_char(p10);
  player->add_char(p11);

  std::shared_ptr<CurseKeyBoard> ckb = std::make_shared<CurseKeyBoard> ();
  auto pc1 = std::make_shared<PlayerControlled> (PlayerControlled(ckb));
  player->add(pc1);
  StraightLine sl{-1, 1};
  auto slp = std::make_shared<StraightLine> (sl);
  ball->add(slp);
}

void Client::clearLevel() {
  model->detach_all();
  bricks.clear();
  ball->clear_map();
  player->clear_map();
  wall->clear_map();
}

void Client::initLevel2() {

  for (size_t i = 0; i < player->size(); ++i) {
    player->at_index(i)->setY(29+i);
    player->at_index(i)->setX(20);
  }
  ball->setX(19);
  ball->setY(34);

  auto bounce = std::make_shared<Collide> ();
  auto damage = std::make_shared<Collide> ();
  bounce->setState(CollideStateType::Bounce);
  ball->add(player,bounce);
  ball->add(wall,bounce);
  
  for (int i = 11; i < 73; i += 18) {
    for (int j = 3; j < 19; j += 8) {
    Rectangle rec{j, i,'x', 6, 2};
    auto x1 = std::make_shared<SingleChar> (j,i-1,'|');
    auto x2 = std::make_shared<SingleChar> (j+1,i-1,'|');
    //auto x13 = std::make_shared<SingleChar> (j+1,i,'+');
    //auto x3 = std::make_shared<SingleChar> (j+2,i-1,'+');
    auto x4 = std::make_shared<SingleChar> (j,i+6,'|');
    auto x5 = std::make_shared<SingleChar> (j+1,i+6,'|');
    //auto x14 = std::make_shared<SingleChar> (j+1,i+5,'+');
    //auto x6 = std::make_shared<SingleChar> (j+2,i+6,'+');

    
    auto x7 = std::make_shared<SingleChar> (j+2,i,'-');
    auto x8 = std::make_shared<SingleChar> (j+2,i+1,'-');
    auto x9 = std::make_shared<SingleChar> (j+2,i+2,'-');
    auto x10 = std::make_shared<SingleChar> (j+2,i+3,'-');
    auto x11 = std::make_shared<SingleChar> (j+2,i+4,'-');
    auto x12 = std::make_shared<SingleChar> (j+2,i+5,'-');

    wall->add_char(x1);
    wall->add_char(x2);
    //wall->add_char(x3);
    wall->add_char(x4);
    wall->add_char(x5);
    //wall->add_char(x6);
    wall->add_char(x7);
    wall->add_char(x8);
    wall->add_char(x9);
    wall->add_char(x10);
    wall->add_char(x11);
    wall->add_char(x12);
    //wall->add_char(x13);
    //wall->add_char(x14);


    auto rptr = std::make_shared<Rectangle> (rec);
    bounce = std::make_shared<Collide> ();
    bounce->setState(CollideStateType::Bounce);
    damage = std::make_shared<Collide> ();
    damage->setState(CollideStateType::Damage);
    rptr->add(ball, damage);
    ball->add(rptr, bounce);
    addBrick(rptr);
    }
  }

}


void Client::initLevel1() {

  for (size_t i = 0; i < player->size(); ++i) {
    player->at_index(i)->setY(29+i);
    player->at_index(i)->setX(20);
  }
  ball->setX(19);
  ball->setY(34);

  auto bounce = std::make_shared<Collide> ();
  auto damage = std::make_shared<Collide> ();
  bounce->setState(CollideStateType::Bounce);
  ball->add(player,bounce);
  
  for (int i = 3; i < 73; i += 8) {
    for (int j = 3; j < 16; j += 5) {
    Rectangle rec{j, i,'x', 6, 2};
    auto rptr = std::make_shared<Rectangle> (rec);
    bounce = std::make_shared<Collide> ();
    bounce->setState(CollideStateType::Bounce);
    damage = std::make_shared<Collide> ();
    damage->setState(CollideStateType::Damage);
    rptr->add(ball, damage);
    ball->add(rptr, bounce);
    addBrick(rptr);
    }
  }

}


void Client::update() { ++level; }

void Client::add_all() { 
  model->add(player);
  model->add(wall);
  model->add(ball);
  for (auto &brick:bricks) model->add(brick);
}

bool Client::win() {
  for (auto &rec:bricks) {
    if (!(rec->getState().type == CollideStateType::Damage)) return false;
  }
  return true;
}




void Client::go() {
  switch (level) {
    case 1: {
      if (!start) {
        int c = getch();
        model->init();
        init();
      } else {
        clearLevel();
      }
      initLevel1();
      add_all();
      start = true;
      break;
    }
    case 2: {
      if (!start) {
        int c = getch();
        model->init();
        init();
      } else {
        clearLevel();
      }
      initLevel2();
      add_all();
      start = true;
      break;
    }
    default:
      break;
  }
}


