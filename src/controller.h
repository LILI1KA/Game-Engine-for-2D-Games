#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "action.h"
#include "subject.h"

class Controller {
    virtual ACTION action() = 0;
    virtual void move(Subject &subject, ACTION action) = 0;
  public:
    ACTION getAction();
    void controller_move(Subject &subject);
    virtual ~Controller() = default;
};


#endif
