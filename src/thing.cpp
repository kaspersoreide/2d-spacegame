#include "thing.h"
#include <math.h>
#include "loadshaders.h"
#include "vertexarrays.h"
#include <glm/gtx/matrix_transform_2d.hpp>

GLuint Thing::program;

Thing::Thing(vec2 pos, vec2 vel, float radius, float spin, vec3 color)
	: pos(pos), vel(vel), radius(radius), spin(spin), color(color) {
	HP = radius * 100;
	maxHP = HP;
	mass = radius * radius;
	Rotation = mat3(1.0f);
    if (!program) {
	    program = loadShaders("shaders/thing/vert.glsl", "shaders/thing/frag.glsl");
    }
}

void Thing::move() {
	pos += vel;
    pos = mod(pos, 10.0f);
	Rotation = rotate(Rotation, spin);
    Model = mat3(
        Rotation[0][0], Rotation[0][1], 0.0f,
        Rotation[1][0], Rotation[1][1], 0.0f,
        pos[0], pos[1], 1.0f
    );
}

void Thing::moveReverse() {
    pos -= vel;
	Rotation = rotate(Rotation, -spin);
    Model = mat3(
        Rotation[0][0], Rotation[0][1], 0.0f,
        Rotation[1][0], Rotation[1][1], 0.0f,
        pos[0], pos[1], 1.0f
    );
}


void Thing::gravitate(Thing* other) {
    const float G = 0.00003f;
    vec2 dir = other->pos - pos;
    vec2 force = G * dir / dot(dir, dir);
    vel += force;
    other->vel -= force;
}

void Thing::loadVAO() {
	VAO = vertexArray1x2f(vertices);
}

void Thing::render() {
	glBindVertexArray(VAO);
	glUseProgram(program);
	GLuint l_M = glGetUniformLocation(program, "Model");
	glUniformMatrix3fv(l_M, 1, GL_FALSE, &Model[0][0]);
	GLuint l_c = glGetUniformLocation(program, "color");
	glUniform3fv(l_c, 1, &color[0]);
    glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
}

void Thing::render(vec2 camerapos) {
    glBindVertexArray(VAO);
	glUseProgram(program);
    mat3 MV = Model;
    MV[2][0] -= camerapos[0];
    MV[2][1] -= camerapos[1]; 
	GLuint l_M = glGetUniformLocation(program, "Model");
	glUniformMatrix3fv(l_M, 1, GL_FALSE, &MV[0][0]);
	GLuint l_c = glGetUniformLocation(program, "color");
	glUniform3fv(l_c, 1, &color[0]);
    glDrawArrays(GL_LINE_LOOP, 0, vertices.size());
}

bool Thing::detectCollission(Thing* other) {
	float minDist = length(other->pos - pos);
	if (minDist > other->radius + radius) return false;
	//find closest edge point on other
	vec2 closestPoint = other->pos;
	for (vec2 v : other->vertices) {
		vec2 p = other->Model * vec3(v[0], v[1], 1.0f);
		if (dot(p - pos, p - pos) < dot(closestPoint - pos, closestPoint - pos)){
			closestPoint = p;
		}
	}
    for (int i = 0; i < collisionHulls.size(); i++) {	
        bool found = true;
	    for (auto it = collisionHulls[i].begin(); it != collisionHulls[i].end(); ++it) {
            vec2 p1 = Model * vec3((*it)[0], (*it)[1], 1.0f);
	    	vec2 dist = closestPoint - p1;
	    	auto jt = it;
        	if (it == collisionHulls[i].begin()) {
	    		jt = collisionHulls[i].end() - 1;
	    	}
	    	else {
	    		jt = it - 1;
	    	}
            vec2 p2 = Model * vec3((*jt)[0], (*jt)[1], 1.0f);
	       	vec2 edgeVec = p1 - p2;
            vec2 normal(edgeVec[1], -edgeVec[0]);
        	if (dot(normal, dist) > 0.0f) {
                found = false;
                break;
            }
	    }
        if (found) return true;
    }
	return false;
}

void Thing::collide(Thing* other) {
    if (!detectCollission(other) && !other->detectCollission(this)) return;
	moveReverse();
	other->moveReverse();

	vec2 poc = pos + radius * (other->pos - pos) / length(other->pos - pos);
	vec2 r1 = poc - pos;
	vec2 r2 = poc - other->pos;
	vec2 totalVelThis = 0.9f * (vel + spin * vec2(r1[1], -r1[0]));
	vec2 totalVelThat = 0.9f * (other->vel + other->spin * vec2(r2[1], -r2[0]));
	vec2 dv = totalVelThat - totalVelThis;
	//if (dot(r1, dv) > 0) return;

	float m1 = 2 * other->mass / (mass + other->mass);
	float m2 = 2 * mass / (mass + other->mass);
	totalVelThis += m1 * dv;
	totalVelThat -= m2 * dv;

	float dp1 = dot(totalVelThis, r1);
	vel = dp1 * r1 / dot(r1, r1);
	float dp2 = dot(totalVelThat, r2);
	other->vel = dp2 * r2 / dot(r2, r2);

	vec2 tangentThis = totalVelThis - vel;
	spin = (tangentThis[0] * r1[1] - tangentThis[1] * r1[0]) / dot(r1, r1);
	vec2 tangentThat = totalVelThat - other->vel;
	other->spin = (tangentThat[0] * r2[1] - tangentThat[1] * r2[0]) / dot(r2, r2);

    //remove HP
    HP -= maxHP;//300 * other->mass * dot(dv, dv);
    other->HP -= other->maxHP;//300 * mass * dot(dv, dv);
}

Thing::~Thing() {
    glDeleteVertexArrays(1, &VAO);
}
