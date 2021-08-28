#include "ship.h"
#include <math.h>

Ship::Ship() : Thing(vec2(0.0f), vec2(0.0f), 0.1f, 0.0f, vec3(1.0f, 0.0f, 0.0f)) {
	vertices.push_back(vec2(0.0f));
	for (int i = 0; i < 3; i++){
		float c = std::cos((2 * (i - 1) * M_PI) / 3);
		float s = std::sin((2 * (i - 1) * M_PI) / 3);
		vertices.push_back(radius * vec2(c, s));
	}
	vertices.push_back(vec2(0.0f));
	loadVAO();
}
