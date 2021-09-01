#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>
#define SPACE_SIZE 10.0f

using namespace glm;

class Thing {
public:
    Thing() {}
    Thing(vec2 pos, vec2 vel, float size, float spin, vec3 color);
    ~Thing();
    int HP, maxHP;
    mat3 Rotation, Model;
    vec2 pos, vel;
    vec3 color;
    float spin, radius, mass;
	std::vector<vec2> vertices;
    std::vector<std::vector<vec2>> collisionHulls;
    bool detectCollission(Thing* other);
    void gravitate(Thing* other);
    void collide(Thing* other);
	void render();
    void render(vec2 camerapos);
	void move();
    void moveReverse();
protected:
    GLuint VAO;
    static GLuint program;
	void loadVAO();	
};
