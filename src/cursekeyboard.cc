#include "cursekeyboard.h"

CurseKeyBoard::CurseKeyBoard() {
    setlocale(LC_ALL, "");
    mapping['w'] = ACTION::SHOOTUP;
    mapping['s'] = ACTION::SHOOTDOWN;
    mapping['a'] = ACTION::SHOOTLEFT;
    mapping['d'] = ACTION::SHOOTRIGHT;
    mapping[KEY_UP] = ACTION::UP;
    mapping[KEY_DOWN] = ACTION::DOWN;
    mapping[KEY_LEFT] = ACTION::LEFT;
    mapping[KEY_RIGHT] = ACTION::RIGHT;
}

ACTION CurseKeyBoard::action() {
    int n = getch();
    //while ( (n = getch()) == ERR ) continue;
    if (mapping.find(n)  != mapping.end()) return mapping[n];
    return ACTION::INVALID;
}

void CurseKeyBoard::move(Subject &subject, ACTION action) {
    Object *obj = dynamic_cast<Object*> (&subject);
    obj->setAction(action);
    SingleChar *s_ptr = dynamic_cast<SingleChar*> (obj);
    Rectangle *r_ptr = dynamic_cast<Rectangle*> (obj);
    Bitmap *b_ptr = dynamic_cast<Bitmap*> (obj);
    if (s_ptr) {
        switch (action) {
            case ACTION::UP: {
                s_ptr->setX(s_ptr->getX()-1);
                break;
            }
            case ACTION::DOWN: {
                s_ptr->setX(s_ptr->getX()+1);
                break;
            }
            case ACTION::LEFT: {
                s_ptr->setY(s_ptr->getY()-1);
                break;
            }
            case ACTION::RIGHT: {
                s_ptr->setY(s_ptr->getY()+1);
                break;
            }
            default: {
                break;
            }
        }
    }
    if (r_ptr) {
        switch (action) {
            case ACTION::UP: {
                r_ptr->setX(r_ptr->getX()-2);
                break;
            }
            case ACTION::DOWN: {
                r_ptr->setX(r_ptr->getX()+2);
                break;
            }
            case ACTION::LEFT: {
                r_ptr->setY(r_ptr->getY()-2);
                break;
            }
            case ACTION::RIGHT: {
                r_ptr->setY(r_ptr->getY()+2);
                break;
            }
            default: {
                break;
            }
        }
    }
    if (b_ptr) {
        switch (action) {
            case ACTION::UP: {
                for (size_t i = 0; i < b_ptr->size(); ++i) b_ptr->at_index(i)->setX(b_ptr->at_index(i)->getX()-1);
                break;
            }
            case ACTION::DOWN: {
                for (size_t i = 0; i < b_ptr->size(); ++i) b_ptr->at_index(i)->setX(b_ptr->at_index(i)->getX()+1);
                break;
            }
            case ACTION::LEFT: {
                for (size_t i = 0; i < b_ptr->size(); ++i) b_ptr->at_index(i)->setY(b_ptr->at_index(i)->getY()-4);
                break;
            }
            case ACTION::RIGHT: {
                for (size_t i = 0; i < b_ptr->size(); ++i) b_ptr->at_index(i)->setY(b_ptr->at_index(i)->getY()+4);
                break;
            }
            default: {
                break;
            }
        }
    }

}


