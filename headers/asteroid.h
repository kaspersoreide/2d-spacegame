#pragma once

#include "thing.h"

class Asteroid : public Thing {
public:
    Asteroid();
    Asteroid(vec2 playerPos);
};