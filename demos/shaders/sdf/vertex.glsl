#version 330 core

layout(location = 0) in vec3 position;

out vec2 frag_uv; // Full-screen UV coords [-1, 1]

void main() {
    gl_Position = vec4(position, 1.0);
    frag_uv = position.xy;
}