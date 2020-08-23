#include <glm/glm.hpp>

using namespace glm;

class Thing {
public:
    Thing(vec2 pos, vec2 vel, float size, float spin, vec3 color);
    int HP, maxHP;
    vec2 pos, vel;
    float spin, size, mass;
};