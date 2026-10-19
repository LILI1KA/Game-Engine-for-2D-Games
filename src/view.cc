#include <ncurses.h>
#include <memory>
#include "view.h"
#include "border.h"
#include "status.h"
#include "subject.h"
#include "object.h"

View::View(): border{std::make_shared<Border>()}, status{std::make_shared<Status>()}, win{nullptr} {}

View::~View() { delwin(win); }


void View::add(std::shared_ptr<Subject> obj) {
    Subject &subject = *obj;
    Object *ob_ptr = dynamic_cast<Object*> (&subject);
    SingleChar *single_char = dynamic_cast<SingleChar*> (ob_ptr);
    Rectangle *rectangle = dynamic_cast<Rectangle*> (ob_ptr);
    Bitmap *bitmap = dynamic_cast<Bitmap*> (ob_ptr);
    if (single_char) {
        int x = single_char->getX();
        int y = single_char->getY();
        char c = single_char->getChar();
        if ( x > 0 && x < 21 && y > 0 && y < 79 ) put(x, y, c);
    } else if (rectangle) {
        int x = rectangle->getX();
        int y = rectangle->getY();
        int width = rectangle->getWidth();
        int length = rectangle->getLength();
        char c = rectangle->getChar();
        /*
        for (size_t i = 0; i < width; ++i) {
            put(x+i, y, c);
            put(x+i, y+length-1, c);
        }
        for (size_t j = 0; j < length; ++j) {
            put(x, y+j, c);
            put(x+width-1, y+j, c);
        }
        */
        for (size_t i = x; i < x+width; ++i) {
            for (size_t j = y; j < y+length; ++j) if ( i > 0 && i < 21 && j > 0 && j < 79 ) put(i,j,c);
        }
    } else if (bitmap) {
        for (size_t i = 0; i < bitmap->size(); ++i) {
            add(bitmap->at_index(i));
        }
    }
}

void View::init() { 
    initscr();
    keypad(stdscr, TRUE);
    win = newwin(25, 80, 0, 0);
}

void View::erase() { werase(win); }

void View::put(int x, int y, char c) { mvwaddch(win, x, y, c); }

void View::display_border() {
    mvwaddch(win, 0, 0, '+');
    mvwaddch(win, 0, 79, '+');
    mvwaddch(win, 21, 0, '+');
    mvwaddch(win, 21, 79, '+');
    for(size_t i = 1; i < 79; ++i ) {
        mvwaddch(win, 0, i, '-');
        mvwaddch(win, 21, i, '-');
    }
    for(size_t i = 1; i < 21; ++i ) {
        mvwaddch(win, i, 0, '|');
        mvwaddch(win, i, 79, '|');
    }
}

void View::display_status() {
    std::string line_1, line_2, line_3;
    status->get_line(line_1, line_2, line_3);
    mvwaddstr(win, 22, 0, line_1.c_str());
    mvwaddstr(win, 23, 0, line_2.c_str());
    mvwaddstr(win, 24, 0, line_3.c_str());
}

void View::display() {
    display_border();
    display_status();
    wrefresh(win);
}

std::shared_ptr<Border> View::get_border() { return border; }

std::shared_ptr<Status> View::get_status() { return status; }

WINDOW *View::get_window() { return win; }

