#include "asteroid.h"
#include <math.h>
#include "random.h"

Asteroid::Asteroid() {
    pos = rng::randvec2();
    vel = vec2(0.0f);
    radius = rng::randf(0.0f, 0.2f);
    spin = rng::randf(-0.008f, 0.008f);
    color = vec3(rng::randf(), rng::randf(), rng::randf());
    HP = radius * 100;
	maxHP = HP;
	mass = radius * radius;
    Rotation = mat3(1.0f);
    int numVertices = rng::randi(5, 20);
    for (int i = 0; i < numVertices; i++) {
        float c = std::cos((2 * i * M_PI) / numVertices);
		float s = std::sin((2 * i * M_PI) / numVertices);
		vertices.push_back(radius * vec2(c, s));
    }

    loadVAO();
}