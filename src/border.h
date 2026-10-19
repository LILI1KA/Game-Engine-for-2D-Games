#ifndef BORDER_H
#define BORDER_H
#include <ncurses.h>
#include "border_state.h"

class Border {
    BorderState b_state; //true for solid, false for pass
  public:
    Border();
    Border(BorderState b_state);
    BorderState get_state();
    void set_state(BorderStateType);
};


#endif
