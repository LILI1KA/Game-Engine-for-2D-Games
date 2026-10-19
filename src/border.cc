#include <ncurses.h>
#include "border.h"

Border::Border(): b_state{BorderState()} {}

Border::Border(BorderState b_state): b_state{b_state} {}

BorderState Border::get_state() { return b_state; }

void Border::set_state(BorderStateType type) { b_state = BorderState{type}; }


