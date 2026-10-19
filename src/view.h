#ifndef VIEW_H
#define VIEW_H
#include <ncurses.h>
#include "border.h"
#include "status.h"
#include "subject.h"
#include "object.h"
#include <memory>

class View {
    std::shared_ptr<Border> border;
    std::shared_ptr<Status> status;
    WINDOW *win;
  public:
    View();
    ~View();
    void add(std::shared_ptr<Subject> obj);
    void init();
    void erase();
    void put(int x, int y, char c);
    void display_border();
    void display_status();
    void display();
    std::shared_ptr<Border> get_border();
    std::shared_ptr<Status> get_status();
    WINDOW *get_window();
};


#endif
