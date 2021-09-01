#include "asteroid.h"
#include <math.h>
#include "random.h"
#include "convexhull.h"

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
    //int numVertices = 5;//rng::randi(5, 20);
    //for (int i = 0; i < numVertices; i++) {
    //    float c = std::cos((2 * i * M_PI) / numVertices);
	//	float s = std::sin((2 * i * M_PI) / numVertices);
	//	vertices.push_back(radius * vec2(c, s));
    //}
    for (int i = 0; i < 50; i++) {
        points.push_back(radius * rng::randf() * rng::randvec2());
    }
    vertices = convexHull(points);
    loadVAO();
    collisionHulls.push_back(vertices);
}

Asteroid::Asteroid(vec2 playerPos) : Asteroid() {
    pos = 4.0f * rng::randvec2() + playerPos;
}

Asteroid::Asteroid(std::vector<vec2> _points, mat3 _Rotation, vec2 _pos, vec2 _vel, float _spin) {
    for (auto it = _points.begin(); it < _points.end(); ++it) {
        *it = _Rotation * vec3(it->x, it->y, 0.0f);
    }
    points = _points;
    //find new center
    vec2 center(0.0f);
    for (vec2 p : points) {
        center += p;
    }
    center /= points.size();
    vel = 0.2f * center + _vel + _spin * vec2(center[1], -center[0]);
    spin = _spin - _spin * (center[1] - center[0]);
    pos = _pos + center;
    radius = 0.0f;
    for (auto it = points.begin(); it < points.end(); ++it) {
        *it -= center;
        radius = max(radius, length(*it));
    }
    Rotation = mat3(1.0f);
    vertices = convexHull(points);
    loadVAO();
    collisionHulls.push_back(vertices);
    HP = radius * 100;
	maxHP = HP;
	mass = radius * radius;
}

void Asteroid::split(Asteroid **a1, Asteroid **a2) {
    vec2 splitDir = rng::randvec2();
    std::vector<vec2> points1, points2;
    for (vec2 p : points) {
        if (dot(p, splitDir) > 0.0f) points1.push_back(p);
        else points2.push_back(p);
    }
    *a1 = new Asteroid(points1, Rotation, pos, vel, spin);
    *a2 = new Asteroid(points2, Rotation, pos, vel, spin);
    (*a1)->color = color;
    (*a2)->color = color;
}
