#version 430

layout(location = 0) in vec2 pos;

uniform mat3 Model;

vec3 torusMap(vec2 planePos, float radius, float thickness) {
    //based on https://mathworld.wolfram.com/Torus.html
    const float PI = 3.14159265;
    float u = 2.0 * PI * planePos.x;
    float v = 2.0 * PI * planePos.y;
    return vec3(
        (radius + thickness * cos(v)) * cos(u),
        (radius + thickness * cos(v)) * sin(u),	
        thickness * sin(v)
    );
}

void main() {
    vec3 realPos = Model * vec3(pos, 1.0);
    gl_Position = vec4(realPos, 1.0);
}