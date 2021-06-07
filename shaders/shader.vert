#version 330 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 tex;

out vec2 tex_coords;

uniform mat4 model;
uniform mat4 projection;

void main() {
    tex_coords = tex;
    gl_Position = vec4(vertex, 0.0, 1.0);
}
