#version 330 core

layout(location = 0) in vec3 position; // Vertex position in object space.
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal; // Vertex normal in object space.
layout(location = 3) in vec2 tex_coord;
layout(location = 4) in vec2 tangent;   // Vertex tangent in object space.
layout(location = 5) in vec2 bitangent; // Vertex bitangent in object space.

out vec4 fragment_position; // Position in world space.
out vec4 fragment_normal;   // Surface normal in world space.
out vec3 fragment_color;    // Base color.
out vec2 fragment_tex_coord;
out vec4 light_direction; // Direction light reflects off surface onto camera.

uniform mat4 MVP;            // Model-View-Projection matrix.
uniform vec4 light_position; // Light position in world space.

void main() {
  gl_Position = MVP * vec4(position, 1);

  fragment_position = vec4(position, 1);
  fragment_normal = vec4(normal, 1);
  fragment_normal = normalize(fragment_normal);

  fragment_color = color;
  fragment_tex_coord = tex_coord;

  light_direction = light_position - vec4(position, 1);
  light_direction = normalize(light_direction);
}
