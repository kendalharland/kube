#version 330 core

// =============================================================================
// FRAGMENT SHADER — SDF RAYMARCHER
//
// After the vertex shader runs, the GPU rasterizes each triangle into fragments
// (roughly: pixels). It runs this fragment shader once per fragment, giving us
// the interpolated frag_uv for that pixel's screen position.
//
// Our job here is to figure out what color that pixel should be. We do that
// by simulating a ray of light travelling from the camera through that pixel
// into the scene, and seeing what it hits.
//
// The overall flow is:
//   1. Reconstruct the 3D ray for this pixel           (main)
//   2. March the ray through the scene until it hits   (march)
//   3. If it hit something, compute a surface normal   (calcNormal)
//   4. Shade the hit point with simple lighting        (main)
//   5. Write the final color to FragColor              (main)
// =============================================================================

// 'in' from the vertex shader.
in vec2 frag_uv; // screen-space UV in [-1, 1].
in vec4 frag_normal; // fragment normal vector.
in vec2 frag_tex_coord;

// The one required output: the RGBA color for this pixel.
// We declared it as vec3 (RGB) so the alpha channel defaults to 1.0 (opaque).
out vec4 FragColor;

// -----------------------------------------------------------------------------
// Uniforms
//
// Uniforms are values set from C++ that stay constant for the entire draw call.
// They're the bridge between your engine and the shader. In model.cpp, inside
// Model::Draw(), you'll see:
//
//   shader.SetUniform3f("u_camera_pos", camera->GetPosition());
//   shader.SetUniform2f("u_resolution", glm::vec2(1600, 1200));
//   shader.SetUniform1f("u_time",       (float)glfwGetTime());
//
// OpenGL looks up each uniform by name string and writes it into GPU memory
// before the draw call executes. Every fragment then reads the same values.
// -----------------------------------------------------------------------------
uniform vec3  u_camera_pos;  // world-space camera position, from Camera::GetPosition()
uniform vec3  u_camera_tgt;  // Point the camera looks at
uniform vec2  u_resolution;  // viewport size in pixels, for aspect-ratio correction
uniform float u_time;        // seconds since startup, for animation

// Samplers for lighting.
// For now, we only support one type of each sampler.
uniform sampler2D sampler_ambient;
uniform sampler2D sampler_diffuse;
uniform sampler2D sampler_specular;
uniform vec4 light_color; // Light's diffuse and specular contribution.

// =============================================================================
// SIGNED DISTANCE FUNCTIONS (SDFs)
//
// An SDF is a function f(p) that, given any point p in 3D space, returns the
// shortest distance from p to the surface of an object — with a sign:
//   f(p) > 0  →  p is outside the object
//   f(p) = 0  →  p is exactly on the surface
//   f(p) < 0  →  p is inside the object
//
// This is incredibly useful for raymarching: if we're at point p and the SDF
// returns 2.5, we know we can safely step 2.5 units along the ray without
// hitting anything. We can keep doing this until we're very close to 0.
//
// For a sphere centered at the origin with the given radius, the SDF is just:
//   distance from p to center  minus  radius
//   = length(p) - radius
//
// If p is 3 units from origin and radius is 1, we're 2 units outside (> 0).
// If p is 0.5 units from origin and radius is 1, we're 0.5 units inside (< 0).
// =============================================================================
float sdfSphere(vec3 p, float radius) {
    return length(p) - radius;
}

// =============================================================================
// RAYMARCHING
//
// Raymarching (specifically "sphere tracing") works like this:
//
//   Start at the ray origin (ro). Ask the SDF: "how far am I from anything?"
//   Whatever distance d it returns, it's safe to step exactly d units forward
//   along the ray direction (rd) without overshooting a surface.
//   Repeat from the new position until either:
//     (a) d is very small  →  we're close enough to call it a "hit"
//     (b) dist > MAX_DIST  →  the ray escaped the scene, nothing was hit
//
//   Visualized in 2D (side view), stepping toward a sphere:
//
//   ro  .-----.-----.--.-. ~surface
//       ^     ^     ^ ^
//       step  step  step (each step = SDF value at that point)
//
// This is different from classic raytracing, which solves the ray-surface
// intersection analytically. Raymarching trades some precision for the ability
// to render any shape you can write an SDF for — including combinations,
// smooth blends, and fractals.
// =============================================================================
const int   MAX_STEPS = 100;   // max iterations before giving up
const float MAX_DIST  = 100.0; // ray is considered "missed" beyond this distance
const float SURF_DIST = 0.001; // close enough to the surface to count as a hit

float raymarch(vec3 ro, vec3 rd) {
    float dist = 0.0; // total distance travelled along the ray so far

    for (int i = 0; i < MAX_STEPS; i++) {
        vec3  p = ro + rd * dist;        // current position along the ray
        float d = sdfSphere(p, 1.0);     // nearest surface distance from here
        dist   += d;                     // safe to advance by exactly d
        if (d < SURF_DIST || dist > MAX_DIST) break;
    }

    return dist; // if dist >= MAX_DIST, the ray missed everything
}


// =============================================================================
// SURFACE NORMALS VIA CENTRAL DIFFERENCES
//
// To light a surface we need its outward-facing normal vector at the hit point.
// For analytic shapes you could compute this mathematically, but there's a
// general trick that works for ANY SDF: the gradient of the SDF at a point is
// perpendicular to the surface there — i.e., it IS the normal.
//
// We approximate the gradient numerically using central differences:
//   dF/dx ≈ (F(p + ε·x̂) - F(p - ε·x̂)) / 2ε
//
// We skip dividing by 2ε because we immediately normalize the result — the
// magnitude cancels out. A tiny ε (0.001) gives accurate normals without
// introducing visible stepping artifacts.
// =============================================================================
vec3 calcNormal(vec3 p) {
    const float e = 0.001;
    return normalize(vec3(
        sdfSphere(p + vec3(e,0,0), 1.0) - sdfSphere(p - vec3(e,0,0), 1.0), // dF/dx
        sdfSphere(p + vec3(0,e,0), 1.0) - sdfSphere(p - vec3(0,e,0), 1.0), // dF/dy
        sdfSphere(p + vec3(0,0,e), 1.0) - sdfSphere(p - vec3(0,0,e), 1.0)  // dF/dz
    ));
}


// =============================================================================
// MAIN — builds the ray for this pixel, marches it, then shades the result
// =============================================================================
void main() {
    vec3 light_dir = normalize(vec3(1.0, 2.0, 3.0)); // arbitrary directional light

    // -------------------------------------------------------------------------
    // Step 1: Convert this pixel's screen UV into a properly-proportioned 2D
    // coordinate, correcting for the window's aspect ratio.
    //
    // frag_uv comes in as [-1,1] in both X and Y. But pixels aren't square if
    // the window is wider than tall — a point at uv=(1,1) is the top-right
    // corner, but in world space that's not a 45° angle from center if the
    // window is 1600x1200. Multiplying X by (width/height) stretches the
    // horizontal range so that equal distances on screen map to equal angles.
    // -------------------------------------------------------------------------
    float aspect = u_resolution.x / u_resolution.y; // e.g. 1600/1200 ≈ 1.333
    vec2  uv     = frag_uv * vec2(aspect, 1.0);     // uv.x now spans ~[-1.33, 1.33]

    // -------------------------------------------------------------------------
    // Step 2: Build the camera basis vectors for a "look-at" setup.
    //
    // We want every ray to originate at the camera and pass through the scene.
    // To do that we construct three orthogonal vectors that describe the
    // camera's orientation:
    //
    //   fwd   — the direction the camera is facing (toward the target)
    //   right — points to the camera's right (perpendicular to fwd and world-up)
    //   up    — points upward relative to the camera's view
    //
    // This works for any camera position, unlike the old hardcoded (0,0,-1)
    // direction which only worked when the camera sat on the +Z axis.
    //
    // The only degenerate case is when the camera is directly above or below
    // the target (fwd is parallel to world-up), which makes the cross product
    // zero. For this demo the camera is always off to the side so it's fine.
    // -------------------------------------------------------------------------
    vec3 ro     = u_camera_pos;               // ray origin = camera position
    vec3 target = u_camera_tgt;               // point the camera looks at
    vec3 fwd    = normalize(target - ro);     // forward: camera → target
    vec3 right  = normalize(cross(fwd, vec3(0,1,0))); // right: perpendicular to fwd and world-up
    vec3 up     = cross(right, fwd);          // up: completes the right-handed basis

    // -------------------------------------------------------------------------
    // Step 3: Compute the ray direction for this specific pixel.
    //
    // Think of a virtual "image plane" floating in front of the camera at
    // distance `focal` along fwd. Each pixel corresponds to a point on that
    // plane. The ray direction is from the camera origin toward that point.
    //
    //   rd = normalize( fwd * focal          <- center of the image plane
    //                 + right * uv.x          <- shift left/right per pixel
    //                 + up    * uv.y )         <- shift up/down per pixel
    //
    // A larger focal length (e.g. 3.0) makes the image plane farther away,
    // compressing the angular spread of rays — that's a narrower (zoom) FOV.
    // A smaller value (e.g. 0.8) widens the FOV like a wide-angle lens.
    // 1.5 gives roughly 67°, a natural-looking perspective.
    // -------------------------------------------------------------------------
    vec3 rd = normalize(fwd * 1.5 + right * uv.x + up * uv.y);

    // -------------------------------------------------------------------------
    // Step 4: March the ray and interpret the result.
    // -------------------------------------------------------------------------
    float dist = raymarch(ro, rd);

    if (dist < MAX_DIST) {
        // The ray hit the sphere. Reconstruct the exact hit position.
        vec3 p      = ro + rd * dist;
        vec3 normal = calcNormal(p);

        // Simple Lambertian (diffuse) lighting:
        //   brightness = max(dot(normal, light_dir), 0)
        //
        // dot(N, L) is the cosine of the angle between the surface normal and
        // the light direction. When they point the same way (surface faces the
        // light) it's 1.0 (fully lit). At 90° it's 0 (grazing). We clamp
        // negatives to 0 so back-facing surfaces don't go dark-negative.
        float diffuse  = max(dot(normal, light_dir), 0.0);
        float ambient  = 0.1; // small constant so shadowed areas aren't pure black

        vec3 sphereColor = vec3(0.9, 0.5, 1.0); // base blue color of the sphere
        vec3 color = sphereColor * (ambient + diffuse);
        FragColor = vec4(color, 0);
    } else {
        // The ray escaped without hitting anything — draw the background.
        // FragColor = vec3(0.05);
        vec4 tex_ambient = texture(sampler_ambient, frag_tex_coord);
        vec4 tex_diffuse = texture(sampler_diffuse, frag_tex_coord);
        vec4 tex_specular = texture(sampler_specular, frag_tex_coord);

        vec4 N = normalize(frag_normal);
        vec4 L = normalize(vec4(light_dir, 1));
        vec4 diffuse_color = light_color * clamp(dot(N, L), 0.0, 1.0);
        FragColor = (tex_ambient + tex_specular + diffuse_color) * tex_diffuse;
    }
}
