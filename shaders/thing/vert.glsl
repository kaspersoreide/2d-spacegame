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
    //if (length(Model[2]) > 2.0) {
    //    realPos.xy *= 1.0 / (length(realPos.xy) * (1.0 + exp(-(0.1 * dot(realPos.xy, realPos.xy) - 2.0))));
    //}
    gl_Position = vec4(0.2 * realPos, 1.0);
}