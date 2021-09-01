#pragma once
#include <vector>
#include <glm/glm.hpp>

using namespace glm;

std::vector<vec2> convexHull(std::vector<vec2>& points);