#include "arlg_client.h"


void fire_cycle(Subject *subject) {
  Fire *fire = dynamic_cast<Fire*> (&(*subject));
  if (fire) {
    if (fire->getChar() == 'x') {
      fire->setChar('X');
    } else if (fire->getChar() == 'X') {
      fire->setChar('x');
    }
  }
}


Player::Player(): SingleChar{}, health{5} { 
  setChar('p');
  setHeight(10);
  std::shared_ptr<CurseKeyBoard> ckb = std::make_shared<CurseKeyBoard> ();
  auto pc1 = std::make_shared<PlayerControlled> (PlayerControlled(ckb));
  add(pc1);
}

int Player::getHealth() { return health; }

void Player::setHealth(int new_health) { health = new_health; }

void Player::collideWith(std::shared_ptr<Object> object,std::shared_ptr<Collide> collide) {
  if (!is_collision(object)) return;
  Fire *f_ptr = dynamic_cast<Fire*> (&(*object));
  Walker *w_ptr = dynamic_cast<Walker*> (&(*object));
  Boss *b_ptr = dynamic_cast<Boss*> (&(*object));
  if (f_ptr && collide->getState().type == CollideStateType::Pass) {
    setHealth(getHealth()-1);
  } else if (w_ptr && collide->getState().type == CollideStateType::Bounce) {
    setHealth(getHealth()-1);
    Object::collideWith(object, collide);
  } else if (b_ptr && collide->getState().type == CollideStateType::Bounce) {
    setHealth(getHealth()-1);
    Object::collideWith(object, collide);
  } else {
    Object::collideWith(object, collide);
  }
  
}


Fire::Fire(): SingleChar{} { 
  setChar('x');
  setHeight(10);
  Cycle cycle{fire_cycle,3};
  auto cp = std::make_shared<Cycle> (cycle);
  add(cp);
}

void Fire::collideWith(std::shared_ptr<Object> object,std::shared_ptr<Collide> collide) {
  if (!is_collision(object)) return;
  Object::collideWith(object, collide);
}


Bullet::Bullet(): SingleChar() {
  setChar('O');
  setHeight(10);
  Gravitate u{'u'};
  auto up = std::make_shared<Gravitate> (u);
  add(up);
}
    
Bullet::Bullet(char dir) {
  SingleChar();
  setChar('O');
  setHeight(10);
  if (!(dir == 'u' || dir == 'd' || dir == 'l' || dir == 'r')) return;
  detachAll();
  Gravitate n{dir};
  auto np = std::make_shared<Gravitate> (n);
  add(np);
}

void Bullet::setDir(char new_dir) {
  if (!(new_dir == 'u' || new_dir == 'd' || new_dir == 'l' || new_dir == 'r')) return;
  detachAll();
  Gravitate n{new_dir};
  auto np = std::make_shared<Gravitate> (n);
  add(np);
}

void Bullet::collideWith(std::shared_ptr<Object> object,std::shared_ptr<Collide> collide) {
  Object::collideWith(object, collide);
}

HealthPickUp::HealthPickUp(): SingleChar() {
  setChar('h');
  setHeight(10);
}

void HealthPickUp::collideWith(std::shared_ptr<Object> object,std::shared_ptr<Collide> collide) {
  if (!is_collision(object)) return;
  Player *p_ptr = dynamic_cast<Player*> (&(*object));
  if ( p_ptr && collide->getState().type == CollideStateType::Damage ) {
    p_ptr->setHealth(p_ptr->getHealth()+1);
    Object::collideWith(object, collide);
  } else {
    Object::collideWith(object, collide);
  }
}


Exit::Exit(): SingleChar() {
  setChar('e');
  setHeight(0);
}

Walker::Walker(): Rectangle() {
  health = 3;
  setWidth(2);
  setLength(2);
  setHeight(10);
  setChar('a');
}

int Walker::getHealth() { return health; }

void Walker::setHealth(int new_health) { health = new_health; }

void Walker::collideWith(std::shared_ptr<Object> object,std::shared_ptr<Collide> collide) {
  if (!is_collision(object)) return;
  Bullet *b_ptr = dynamic_cast<Bullet*> (&(*object));
  if ( b_ptr && collide->getState().type == CollideStateType::Damage ) {
    setHealth(getHealth()-1);
    if (getHealth() == 0) Object::collideWith(object, collide);
  } else {
    Object::collideWith(object, collide);
  }

}

Boss::Boss(): Bitmap(), health1{10}, health2{15}, rage{false} {
  setX(-1);
  setY(-1);
  for (size_t i = 5; i < 13; i+=2) {
    auto p1 = std::make_shared<SingleChar> (2,i,'-');
    auto p2 = std::make_shared<SingleChar> (11,i,'-');
    add_char(p1);
    add_char(p2);
  }

  auto p3 = std::make_shared<SingleChar> (3,4,',');
  auto p4 = std::make_shared<SingleChar> (3,12,',');
  auto p5 = std::make_shared<SingleChar> (9,3,',');
  auto p6 = std::make_shared<SingleChar> (9,13,',');

  auto p7 = std::make_shared<SingleChar> (4,3,'.');
  auto p8 = std::make_shared<SingleChar> (4,13,'.');
  auto p9 = std::make_shared<SingleChar> (10,4,'.');
  auto p10 = std::make_shared<SingleChar> (10,12,'.');

  auto p11 = std::make_shared<SingleChar> (5,2,'/');
  auto p12 = std::make_shared<SingleChar> (5,14,'\\');
  auto p13 = std::make_shared<SingleChar> (8,2,'\\');
  auto p14 = std::make_shared<SingleChar> (8,14,'/');

  auto p15 = std::make_shared<SingleChar> (6,1,'|');
  auto p16 = std::make_shared<SingleChar> (7,1,'|');
  auto p17 = std::make_shared<SingleChar> (6,15,'|');
  auto p18 = std::make_shared<SingleChar> (7,15,'|');

  auto p19 = std::make_shared<SingleChar> (5,5,'o');
  auto p20 = std::make_shared<SingleChar> (5,11,'o');
  
  auto p21 = std::make_shared<SingleChar> (8,5,'\\');
  auto p22 = std::make_shared<SingleChar> (8,11,'/');
  auto p23 = std::make_shared<SingleChar> (9,6,',');
  auto p24 = std::make_shared<SingleChar> (9,10,',');
  auto p25 = std::make_shared<SingleChar> (9,7,'-');
  auto p26 = std::make_shared<SingleChar> (9,9,'-');



  StraightLine sl{1, 1};
  auto slp = std::make_shared<StraightLine> (sl);
  add(slp);



  add_char(p3);
  add_char(p4);
  add_char(p5);
  add_char(p6);
  add_char(p7);
  add_char(p8);
  add_char(p9);
  add_char(p10);
  add_char(p11);
  add_char(p12);
  add_char(p13);
  add_char(p14);
  add_char(p15);
  add_char(p16);
  add_char(p17);
  add_char(p18);
  add_char(p19);
  add_char(p20);
  add_char(p21);
  add_char(p22);
  add_char(p23);
  add_char(p24);
  add_char(p25);
  add_char(p26);

  setHeights(10);

  auto damage = std::make_shared<Collide> ();
  damage->setState(CollideStateType::Damage);


}

int Boss::getHealth() {
  if (rage) return health2;
  return health1;
}

void Boss::setHealth(int new_health) {
  if (health1 < 0) {
    health2 = new_health;
  } else {
    health1 = new_health;
  }
}

bool Boss::getRage() { return rage; }

void Boss::setRage(bool new_rage) { rage = new_rage; }

void Boss::change_state() {
  if (health1 < 1 && !rage) {
    rage = true;
    --health1;
    clear_all();
    for (size_t i = 5; i < 13; i+=2) {
      auto p1 = std::make_shared<SingleChar> (2,i,'-');
      auto p2 = std::make_shared<SingleChar> (11,i,'-');
      auto p3 = std::make_shared<SingleChar> (3,i,'V');
      add_char(p1);
      add_char(p2);
      add_char(p3);
    }

    auto p3 = std::make_shared<SingleChar> (3,4,',');
    auto p4 = std::make_shared<SingleChar> (3,12,',');
    auto p5 = std::make_shared<SingleChar> (9,3,',');
    auto p6 = std::make_shared<SingleChar> (9,13,',');

    auto p7 = std::make_shared<SingleChar> (4,3,'.');
    auto p8 = std::make_shared<SingleChar> (4,13,'.');
    auto p9 = std::make_shared<SingleChar> (10,4,'.');
    auto p10 = std::make_shared<SingleChar> (10,12,'.');

    auto p11 = std::make_shared<SingleChar> (5,2,'/');
    auto p12 = std::make_shared<SingleChar> (5,14,'\\');
    auto p13 = std::make_shared<SingleChar> (8,2,'\\');
    auto p14 = std::make_shared<SingleChar> (8,14,'/');

    auto p15 = std::make_shared<SingleChar> (6,1,'|');
    auto p16 = std::make_shared<SingleChar> (7,1,'|');
    auto p17 = std::make_shared<SingleChar> (6,15,'|');
    auto p18 = std::make_shared<SingleChar> (7,15,'|');

    auto p19 = std::make_shared<SingleChar> (5,5,'o');
    auto p20 = std::make_shared<SingleChar> (5,11,'o');
  
    auto p21 = std::make_shared<SingleChar> (7,5,',');
    auto p22 = std::make_shared<SingleChar> (7,11,',');

    auto p23 = std::make_shared<SingleChar> (9,6,'.');
    auto p24 = std::make_shared<SingleChar> (9,10,'.');

    auto p25 = std::make_shared<SingleChar> (10,6,',');
    auto p26 = std::make_shared<SingleChar> (10,10,',');

    auto p27 = std::make_shared<SingleChar> (9,7,'-');
    auto p28 = std::make_shared<SingleChar> (9,9,'-');
    
    auto p29 = std::make_shared<SingleChar> (10,7,'-');
    auto p30 = std::make_shared<SingleChar> (10,9,'-');


    add_char(p3);
    add_char(p4);
    add_char(p5);
    add_char(p6);
    add_char(p7);
    add_char(p8);
    add_char(p9);
    add_char(p10);
    add_char(p11);
    add_char(p12);
    add_char(p13);
    add_char(p14);
    add_char(p15);
    add_char(p16);
    add_char(p17);
    add_char(p18);
    add_char(p19);
    add_char(p20);
    add_char(p21);
    add_char(p22);
    add_char(p23);
    add_char(p24);
    add_char(p25);
    add_char(p26);
    add_char(p27);
    add_char(p28);
    add_char(p29);
    add_char(p30);

    

    setHeights(10);
  }
}

void Boss::collideWith(std::shared_ptr<Object> object,std::shared_ptr<Collide> collide) {
  if (!is_collision(object)) return;
  auto b_ptr = dynamic_cast<Bullet*> (&(*object));
  if (b_ptr && collide->getState().type == CollideStateType::Damage) {
    setHealth(getHealth()-1);
    if (getHealth() == 0 && rage) Object::collideWith(object, collide);
  } else {
    Object::collideWith(object, collide);
  }
}

Client::Client(): level{1}, 
                  model{std::make_shared<Model>()}, 
                  player{std::make_shared<Player>()}, 
                  exit{std::make_shared<Exit>()},
                  boss{std::make_shared<Boss>()},
                  bullets{std::vector<std::shared_ptr<Bullet>>()}, 
                  fires{std::vector<std::shared_ptr<Fire>>()}, 
                  healthpicks{std::vector<std::shared_ptr<HealthPickUp>>()},
                  walkers{std::vector<std::shared_ptr<Walker>>()},
                  start{false},
                  walker_cycle{5}
                  {}

std::shared_ptr<Model> Client::getModel() { return model; }

void Client::setModel(std::shared_ptr<Model> new_model) { model = new_model; }

std::shared_ptr<Player> Client::getPlayer() { return player; }

void Client::setPlayer(std::shared_ptr<Player> new_player) { player = new_player; }

std::shared_ptr<Exit> Client::getExit() { return exit; }

void Client::setExit(std::shared_ptr<Exit> new_exit) { exit = new_exit; }

std::shared_ptr<Boss> Client::getBoss() { return boss; }

void Client::setBoss(std::shared_ptr<Boss> new_boss) { boss = new_boss; }

std::vector<std::shared_ptr<Bullet>> &Client::getBullets() { return bullets; }

void Client::setBullets(std::vector<std::shared_ptr<Bullet>> &new_bullets) { bullets = new_bullets; }

std::vector<std::shared_ptr<Fire>> &Client::getFires() { return fires; }

void Client::setFires(std::vector<std::shared_ptr<Fire>> &new_fires) { fires = new_fires; }

std::vector<std::shared_ptr<HealthPickUp>> &Client::getHealthPickUps() { return healthpicks; }

void Client::setHealthPickUps(std::vector<std::shared_ptr<HealthPickUp>> &new_healthpicks) { healthpicks = new_healthpicks; }


void Client::walker_function(Subject* subject) {
  Object *obj = dynamic_cast<Object*> (subject);

}


void Client::add(std::shared_ptr<Bullet> bullet) {
  bullets.emplace_back(bullet);
  for (auto &fire:fires) {
    auto damage1 = std::make_shared<Collide> ();
    damage1->setState(CollideStateType::Damage);
    auto damage2 = std::make_shared<Collide> ();
    damage2->setState(CollideStateType::Damage);
    fire->add(bullet,damage2);
    bullet->add(fire,damage2);
  }
  for (auto &walker:walkers) {
    auto damage1 = std::make_shared<Collide> ();
    damage1->setState(CollideStateType::Damage);
    auto damage2 = std::make_shared<Collide> ();
    damage2->setState(CollideStateType::Damage);
    walker->add(bullet,damage2);
    bullet->add(walker,damage2);
  }
  if (level == 3) {
    auto damage1 = std::make_shared<Collide> ();
    damage1->setState(CollideStateType::Damage);
    auto damage2 = std::make_shared<Collide> ();
    damage2->setState(CollideStateType::Damage);
    boss->add(bullet,damage1);
    bullet->add(boss,damage2);
  }
}

void Client::add(std::shared_ptr<Fire> fire) {
  fires.emplace_back(fire);
  auto pass = std::make_shared<Collide> ();
  pass->setState(CollideStateType::Pass);
  player->add(fire, pass);
}

void Client::add(std::shared_ptr<HealthPickUp> healthpick) { 
  healthpicks.emplace_back(healthpick);
  auto damage = std::make_shared<Collide> ();
  damage->setState(CollideStateType::Damage);
  healthpick->add(player, damage);
}


void Client::add(std::shared_ptr<Walker> walker) {
  walkers.emplace_back(walker);
  auto bounce = std::make_shared<Collide> ();
  bounce->setState(CollideStateType::Bounce);
  walker->add(player, bounce);
  player->add(walker, bounce);

}


void Client::LuckyPick() {
  for (auto it = fires.begin(); it != fires.end(); ++it) {
    if ( (*it)->getState().type == CollideStateType::Damage ) {
      int random = rand() % 100;
      if (random > 80) {
        auto healthpick = std::make_shared<HealthPickUp> ();
        healthpick->setX((*it)->getX());
        healthpick->setY((*it)->getY());
        add(healthpick);
        model->add(healthpick);
      }
      fires.erase(it);
      break;
    }
  }
}

void Client::update_inner() {
  LuckyPick();
  boss->change_state();
  size_t number_of_enemies = 0;
  number_of_enemies += walkers.size();
  if (number_of_enemies == 0 && level != 3) {
    exit->setHeight(player->getHeight());
  } else if (level == 3 && boss->getRage() && boss->getHealth() < 1) {
    exit->setHeight(10);
  }
  for (auto it = walkers.begin(); it != walkers.end(); ++it) {
    if ( (*it)->getState().type == CollideStateType::Damage ) {
      walkers.erase(it);
      break;
    }
  }
  if (walker_cycle == 0) {
    for (auto &walker:walkers) {
        int dx = (player->getX() - walker->getX() > 0)? 1:-1;
        int dy = (player->getY() - walker->getY() > 0)? 1:-1;
        walker->setX(walker->getX()+dx);
        walker->setY(walker->getY()+dy);
    } 
    walker_cycle = 5;
  } else {
    --walker_cycle;
  }
  if (level == 3) {
    std::shared_ptr<Object> obj = std::dynamic_pointer_cast<Object> (boss);
    model->border_collision(boss);
  }
}

int Client::getLevel() { return level; }

void Client::setLevel(int new_level) { level = new_level; }


void Client::init() {
}

void Client::clearLevel() {
  model->detach_all();
  bullets.clear();
  fires.clear();
  healthpicks.clear();
  player->clear_map();
  boss->clear_map();
  walkers.clear();
  /*bricks.clear();
  ball->clear_map();
  player->clear_map();
  wall->clear_map();*/
}




void Client::initLevel1() {
  exit->setHeight(0);
  CollideState pass;
  pass.type = CollideStateType::Pass;
  player->setState(pass);
  player->setX(16);
  player->setY(39);
  exit->setX(8);
  exit->setY(75);

  auto fire1 = std::make_shared<Fire> ();
  fire1->setX(1);
  fire1->setY(59);

  auto fire2 = std::make_shared<Fire> ();
  fire2->setX(20);
  fire2->setY(3);

  auto fire3 = std::make_shared<Fire> ();
  fire3->setX(7);
  fire3->setY(28);

  add(fire1);
  add(fire2);
  add(fire3);

  auto healthpick = std::make_shared<HealthPickUp> ();
  healthpick->setX(12);
  healthpick->setY(61);
  add(healthpick);

}


void Client::initLevel2() {
  exit->setHeight(0);
  int x_coords[7] = {1, 3, 7, 6, 18, 19, 11};
  int y_coords[7] = {23, 29, 71, 3, 53, 61, 59};
  player->setX(16);
  player->setY(39);
  exit->setX(8);
  exit->setY(75);

  for (int i = 0; i < 3; ++i) {
    auto fire = std::make_shared<Fire> ();
    fire->setX(x_coords[i]);
    fire->setY(y_coords[i]);
    add(fire);
  }

  for (int i = 3; i < 7; ++i) {
    auto walker = std::make_shared<Walker> ();
    walker->setX(x_coords[i]);
    walker->setY(y_coords[i]);
    add(walker);
  }

  auto healthpick = std::make_shared<HealthPickUp> ();
  healthpick->setX(2);
  healthpick->setY(39);
  add(healthpick);

}


void Client::initLevel3() {
  exit->setHeight(0);

  auto bounce = std::make_shared<Collide> ();
  boss = std::make_shared<Boss> ();
  bounce->setState(CollideStateType::Bounce);
  player->add(boss,bounce);
  boss->setX(2);
  boss->setY(5);
  player->setX(16);
  player->setY(77);
  exit->setX(8);
  exit->setY(75);

}


void Client::update() { ++level; }

void Client::add_all() {
  if (level == 3) model->add(boss); 
  model->add(player);
  model->add(exit);
  for (auto &bullet:bullets) model->add(bullet);
  for (auto &fire:fires) model->add(fire);
  for (auto &healthpick:healthpicks) model->add(healthpick);
  for (auto &walker:walkers) model->add(walker);
}

bool Client::win() {
  return player->is_collision(exit);
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
    case 3: {
      if (!start) {
        int c = getch();
        model->init();
        init();
      } else {
        clearLevel();
      }
      initLevel3();
      add_all();
      start = true;
      break;
    }
    default:
      break;
  }
}


