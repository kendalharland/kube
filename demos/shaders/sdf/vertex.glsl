#version 330 core

// =============================================================================
// VERTEX SHADER
//
// A shader program is split into two stages: vertex and fragment. OpenGL runs
// this vertex shader once per vertex of the mesh being drawn.
//
// For SDF raymarching we don't want to draw a 3D model — we want to run a ray
// through every pixel on screen. The trick is to draw a single quad (two
// triangles) that covers the entire screen, and then do all the real work in
// the fragment shader, one pixel at a time.
//
// The quad is defined in C++ (shapes.h, quadMesh()) with 6 vertices whose
// positions already sit in Normalized Device Coordinates (NDC):
//
//   NDC space has X and Y in [-1, 1], with (0,0) at the screen center.
//   (-1,-1) is the bottom-left corner; (1,1) is the top-right corner.
//
//   (-1, 1)-----(1, 1)
//      |    \      |
//      |     \     |       <-- two triangles forming a fullscreen quad
//      |      \    |
//   (-1,-1)----(1,-1)
//
// Because the vertices are already in NDC we don't need a model/view/projection
// matrix here — we just pass them straight through to gl_Position.
// (model.cpp still passes out the MVP uniform for this shader for older demos)
//
// We also forward the XY position as a UV coordinate to the fragment shader.
// Since the quad spans [-1,1] in both axes, frag_uv gives us a continuous
// [-1,1] coordinate across the whole screen — exactly what the raymarcher
// needs to know "which direction to shoot this pixel's ray."
// =============================================================================

// 'in' variables are per-vertex inputs fed from the VAO/VBO set up in C++.
// layout(location = 0) must match the attribute index used in vertex_array.cpp
// when glVertexAttribPointer is called for the position attribute.
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_colors;
layout(location = 2) in vec3 v_normal; // Vertex normal in object space.
layout(location = 3) in vec2 v_tex_coord;
layout(location = 4) in vec3 v_tagent;
layout(location = 5) in vec3 v_bitangent;

// 'out' variables are interpolated by the GPU and passed to the fragment shader.
// Because the quad fills the screen, frag_uv smoothly sweeps from (-1,-1) at
// the bottom-left pixel to (1,1) at the top-right pixel.
out vec2 frag_uv;
out vec4 frag_normal;
out vec2 frag_tex_coord;

// Model-View-Projection matrix.
uniform mat4 MVP;

void main() {
    // gl_Position is the built-in output that tells the GPU where this vertex
    // sits on screen. The w component (1.0) means no perspective divide —
    // the XYZ values are used as-is in NDC.
    gl_Position = MVP * vec4(v_position, 1.0);

    // Pass XY through as screen-space UV. Z is irrelevant for 2D screen coords.
    frag_uv = v_position.xy;
    frag_normal = normalize(vec4(v_normal, 1));
    frag_tex_coord = v_tex_coord;
}
