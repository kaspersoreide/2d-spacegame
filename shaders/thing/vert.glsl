#version 430

layout(location = 0) in vec2 pos;

uniform mat3 Model;

void main() {
    gl_Position = vec4(Model * vec3(pos, 1.0), 1.0);
}