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
	Rotation = rotate(Rotation, spin);
    Model = mat3(
        Rotation[0][0], Rotation[0][1], 0.0f,
        Rotation[1][0], Rotation[1][1], 0.0f,
        pos[0], pos[1], 1.0f
    );
}

void Thing::gravitate(Thing* other) {
    const float G = 0.001f;
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

bool Thing::detectCollission(Thing* other) {
	float minDist = length(other->pos - pos);
	if (minDist > other->radius + radius) return false;
	//find closest edge point on other
	vec2 closestPoint = other->pos;
	for (auto v : other->vertices) {
		vec2 p = other->pos + mat2(other->Rotation) * v;
		if (dot(p - pos, p - pos) < dot(closestPoint - p, closestPoint - p)){
			closestPoint = p;
		}
	}	
	for (auto it = vertices.begin(); it < vertices.end(); ++it) {
		vec2 dist = closestPoint - (pos +  mat2(Rotation) * (*it));
		auto jt = it;
    	if (it == vertices.begin()) {
			jt = vertices.end() - 1;
		}
		else {
			jt = it - 1;
		}

	   	vec2 edgeVec = *it - *jt;
    	if (dot(edgeVec, dist) > 0.0f) return false;
	}
	return true;
}

void Thing::collide(Thing* other) {
    if (!detectCollission(other)) return;
    if (!other->detectCollission(this)) return;
	pos -= vel;
	other->pos -= other->vel;

	vec2 poc = pos + radius * (other->pos - pos) / length(other->pos - pos);
	vec2 r1 = poc - pos;
	vec2 r2 = poc - other->pos;
	vec2 totalVelThis = vel + spin * vec2(r1[1], -r1[0]);
	vec2 totalVelThat = other->vel + other->spin * vec2(r2[1], -r2[0]);
	vec2 dv = totalVelThat - totalVelThis;
	if (dot(r1, dv) > 0) return;

	float m1 = 2 * other->mass / (mass + other->mass);
	float m2 = 2 * size / (size + p.size);
	totalVelThis += m1 * dv;
	totalVelThat -= m2 * dv;

	float dp1 = dot(totalVelThis, r1);
	vel = dp1 * r1 / dot(r1, r1);
	float dp2 = dot(totalVelThat, r2);
	p.vel = dp2 * r2 / dot(r2, r2);

	vec3 tangentThis = totalVelThis - vel;
	setSpin(cross(tangentThis, r1) / dot(r1, r1));
	vec3 tangentThat = totalVelThat - p.vel;
	p.setSpin(cross(tangentThat, r2) / dot(r2, r2));
}
