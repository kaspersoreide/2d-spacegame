#pragma once

#include "ship.h"

class Player : public Ship {
public:
    Player();
    void move();
    bool keyUp, keyDown, keyLeft, keyRight; //specifies which keys are currently pressed
};