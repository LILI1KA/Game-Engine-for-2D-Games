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

#include "arlg_client.h"



int main(int argc, char *argv[]) {
    auto model = std::make_shared<Model>();
    auto client = std::make_shared<Client>();
    client->setModel(model);
    int c;
    std::string line1_temp = "current player health: ",line1 = "",
                line2 = "Use arrow keys to move, w,a,s,d to shoot.", 
                line3_temp = "current level: ", line3 = "";
    if (argc > 1) {
        std::string str = argv[1];
        int lev = std::stoi(str);
        if (lev == 1 || lev == 2 || lev == 3) client->setLevel(lev);
    }
    client->go();
    int a = client->getFires().size();
    int b = model->size();
    int health = client->getPlayer()->getHealth();
    line1 += std::to_string(health);
    model->set_status(line1,line2,line3);
    while (true) {

        /*if (client->getBall()->getX() == 20) {
            if (record_health != health) {
                --health;
            } else {
                --record_health;
            }
        }
        line1 = line1_temp+std::to_string(health);
        model->set_status(line1,line2,line3);*/
        model->update();
        client->update_inner();
        int x = client->getPlayer()->getX(), y = client->getPlayer()->getY();
        if (y < 1 || y > 78 || x < 1 || x > 20) client->getPlayer()->reset_all();
        health = client->getPlayer()->getHealth();
        line1 = std::to_string(health);
        if (client->getLevel() == 3) {
            std::string temp = "Boss health: ";
            temp += std::to_string(client->getBoss()->getHealth());
            line3 += temp;
        }
        model->set_status(line1,line2,line3);
        model->display();
        ACTION action = client->getPlayer()->getAction();
        if (action == ACTION::SHOOTUP) {
            auto bullet = std::make_shared<Bullet> ('u');
            bullet->setX(client->getPlayer()->getX());
            bullet->setY(client->getPlayer()->getY());
            client->add(bullet);
            model->add(bullet);
            bullet->setAction(ACTION::NONE);
        } else if (action == ACTION::SHOOTDOWN) {
            auto bullet = std::make_shared<Bullet> ('d');
            bullet->setX(client->getPlayer()->getX());
            bullet->setY(client->getPlayer()->getY());
            client->add(bullet);
            model->add(bullet);
            bullet->setAction(ACTION::NONE);
        } else if (action == ACTION::SHOOTLEFT) {
            auto bullet = std::make_shared<Bullet> ('l');
            bullet->setX(client->getPlayer()->getX());
            bullet->setY(client->getPlayer()->getY());
            client->add(bullet);
            model->add(bullet);
            bullet->setAction(ACTION::NONE);
        } else if (action == ACTION::SHOOTRIGHT) {
            auto bullet = std::make_shared<Bullet> ('r');
            bullet->setX(client->getPlayer()->getX());
            bullet->setY(client->getPlayer()->getY());
            client->add(bullet);
            model->add(bullet);
            bullet->setAction(ACTION::NONE);
        }

        if (client->win()) model->set_win();
        if (health == 0) model->set_lose();
        if (model->state_message() == "Win") {
            model->set_state(EndStateType::Continue);
            model->update();
            model->display();
            model->set_win();
            if (client->getLevel() == 3) {
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
                client->getPlayer()->setHealth(5);
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
                client->getPlayer()->setHealth(5);
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





