#include "asteroid.h"
#include <math.h>
#include "random.h"

Asteroid::Asteroid() {
    pos = rng::randvec2();
    vel = 0.02f * rng::randvec2();
    radius = rng::randf(0.1f, 0.5f);
    spin = rng::randf(-0.008f, 0.008f);
    color = 0.5f * rng::randvec3() + vec3(0.5f);
    HP = radius * 100;
	maxHP = HP;
	mass = radius * radius;
    Rotation = mat3(1.0f);
    int numVertices = 5;//rng::randi(5, 20);
    for (int i = 0; i < numVertices; i++) {
        float c = std::cos((2 * i * M_PI) / numVertices);
		float s = std::sin((2 * i * M_PI) / numVertices);
		vertices.push_back(radius * vec2(c, s));
    }
    loadVAO();
    collisionHulls.push_back(vertices);
    
}

Asteroid::Asteroid(vec2 playerPos) : Asteroid() {
    pos = 3.0f * rng::randvec2() + playerPos;
}