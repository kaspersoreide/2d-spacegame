#pragma once

#include "thing.h"

class Asteroid : public Thing {
public:
    Asteroid();
    Asteroid(vec2 playerPos);
    Asteroid(std::vector<vec2> points, mat3 Rotation, vec2 pos, vec2 vel, float spin);
    void split(Asteroid **a1, Asteroid **a2);
    std::vector<vec2> points;
};