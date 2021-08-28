#pragma once
#include "GL/glew.h"
#include <vector>
#include "glm/glm.hpp"

using namespace std;
using namespace glm;

GLuint vertexArray2x3f(const vector<vec3>& vertices);

GLuint vertexArray1x3f(const vector<vec3>& vertices);

GLuint vertexArray1x2f(vector<vec2>& vertices);
