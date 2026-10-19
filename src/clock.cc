#ifndef BORDER_STATE_H
#define BORDER_STATE_H

#include "clock.h"
#include <chrono>
#include <thread>

Clock::Clock() {}

void Clock::tick() { 
    timeout(50);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}


#endif
