#ifndef CURSE_KEY_BOARD_H
#define CURSE_KEY_BOARD_H

#include "controller.h"
#include <iostream>
#include <string>
#include <map>
#include <ncurses.h>
#include "object.h"

class CurseKeyBoard: public Controller {
    std::map<int,ACTION> mapping;
    ACTION action() override;
    void move(Subject &subject, ACTION action) override;
  public:
    CurseKeyBoard();

};


#endif
