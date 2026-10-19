#include "controller.h"

ACTION Controller::getAction() { return action(); }

void Controller::controller_move(Subject &subject) {
    ACTION action = getAction(); 
    move(subject, action);
}
