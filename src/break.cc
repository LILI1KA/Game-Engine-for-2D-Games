#include <ncurses.h>

#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include <memory>

#include "view.h"
#include "model.h"
#include "object.h"
#include "movement.h"
#include "collide.h"

#include "cursekeyboard.h"

#include "break_client.h"



int main(int argc, char *argv[]) {
    auto model = std::make_shared<Model>();
    auto client = std::make_shared<Client>();
    client->setModel(model);
    int c;
    std::string line1_temp = "Try breaking all the bricks before health becomes 0! current_health: ",line1 = "",
                line2 = "All the four arrow keys can be used to move the platform.", 
                line3_temp = "current level: ", line3 = "";
    int health = 5, record_health = health;
    if (argc > 1) {
        std::string str = argv[1];
        int lev = std::stoi(str);
        if (lev == 1 || lev == 2) client->setLevel(lev);
    }
    client->go();
    while (true) {


        if (client->getBall()->getX() == 20) {
            if (record_health != health) {
                --health;
            } else {
                --record_health;
            }
        }
        line1 = line1_temp+std::to_string(health);
        model->set_status(line1,line2,line3);
        model->update();
        model->display();
        if (client->win()) model->set_win();
        if (health == 0) model->set_lose();
        if (model->state_message() == "Win") {
            model->set_state(EndStateType::Continue);
            model->update();
            model->display();
            model->set_win();
            if (client->getLevel() == 2) {
                line3 = "You win!!! Enter any key to quit";
                model->set_status(line1,line2,line3);
                model->display();
                timeout(50000);
                int end = getch();
                break;
            }
            line3 = "You win, enter n to enter next level, or q to quit";
            model->set_status(line1,line2,line3);
            model->display();
            LABEL1:
            int end = getch();
            if (end == 'n') {
                model->detach_all();
                health = 5;
                model->set_state(EndStateType::Continue);
                client->update();
                client->go();
                continue;
            } else if (end == 'q') {
                break;
            } else {
                goto LABEL1;
            }
        } else if (model->state_message() == "Lose") {
            line3 = "You lose, enter r to restart level, or q to quit";
            model->set_status(line1,line2,line3);
            model->display();
            LABEL2:
            int end = getch();
            if (end == 'r') {
                model->detach_all();
                health = 5;
                model->set_state(EndStateType::Continue);
                client->go();
                continue;
            } else if (end == 'q') {
                break;
            } else {
                goto LABEL2;
            }
        } else {
            line3 = line3_temp + "level " + std::to_string(client->getLevel()) + ".";
            model->set_status(line1,line2,line3);
        }
    }
    client->clearLevel();
    endwin();
    return 0;
}




