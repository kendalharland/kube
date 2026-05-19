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
// This is just the xy coordinates of the fragment in screen-space which
// spans [-1, 1] inclusive on the x and y axes.
in vec2 frag_uv; 

// The output RGBA color for this pixel.
out vec4 FragColor;

// -----------------------------------------------------------------------------
// Uniforms
//
// Uniforms are values set from C++ that stay constant for the entire draw call.
// They're the bridge between the engine and the shader. In model.cpp, inside
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

// =============================================================================
// MATRIX TRANSFORMS
//
// Useful transforms for simulating motion.
// =============================================================================
mat4 rotation3d(vec3 axis, float angle) {
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat4(
        oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
        oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
        oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
        0.0,                                0.0,                                0.0,                                1.0
    );
}

vec3 rotate(vec3 v, vec3 axis, float angle) {
  mat4 m = rotation3d(axis, angle);
  return (m * vec4(v, 1.0)).xyz;
}

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
float opUnion(float a, float b) {
    return min(a, b);
}

float opSubtraction(float a, float b) {
    return max(a, b);
}

float opSmoothUnion( float d1, float d2, float k )
{
    k *= 4.0;
    float h = max(k-abs(d1-d2),0.0);
    return min(d1, d2) - h*h*0.25/k;
}

float sdSphere(vec3 p, float radius) {
    return length(p) - radius;
}

float sdTorus( vec3 p, vec2 t ) {
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}

float sdHexPrism( vec3 p, vec2 h )
{
  const vec3 k = vec3(-0.8660254, 0.5, 0.57735);
  p = abs(p);
  p.xy -= 2.0*min(dot(k.xy, p.xy), 0.0)*k.xy;
  vec2 d = vec2(
       length(p.xy-vec2(clamp(p.x,-k.z*h.x,k.z*h.x), h.x))*sign(p.y-h.x),
       p.z-h.y );
  return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

float sdSin(vec3 p) {
  return 1.0 - (sin(p.x) + sin(p.y) + sin(p.z))/3.0;
}

float sdScene(vec3 p) {
    float d = 1e10;
    vec3 axis = vec3(10 * sin(u_time), -15 * cos(u_time), 50*sin(u_time));

    // Torus
    {
        vec3 center = vec3(0);
        p = rotate(p + center, -axis, u_time * 0.9);
        float dTorus = sdTorus(p, vec2(3, .4));
        d = opSmoothUnion(d, dTorus, 0.1);
    }
    {
        vec3 center = vec3(0);
        p = rotate(p + center, axis, u_time * 0.8);
        float dTorus = sdTorus(p, vec2(3, .4));
        d = opSmoothUnion(d, dTorus, 0.1);
    }
    // Hex prism
    {
        vec3 center = vec3(0);
        p = rotate(p + center, axis, u_time * 0.7);
        float dHexPrism = sdHexPrism(p, vec2(.5, 7));
        d = opSmoothUnion(d, dHexPrism, 0.1);
    }
    // Sin wave
    {
        float scale = 8.0 + 6.0 * sin(u_time * 0.1);
        float dSin = (0.8 - sdSin(p * scale))/(scale * 2.0);
        d = opSubtraction(d, dSin);
    }

    return d;
}

float softShadows(vec3 ro, vec3 rd, float mint, float maxt, float k ) {
  float resultingShadowColor = 1.0;
  float t = mint;
  for(int i = 0; i < 50 && t < maxt; i++) {
      float h = sdScene(ro + rd*t);
      if( h < 0.001 )
          return 0.0;
      resultingShadowColor = min(resultingShadowColor, k*h/t );
      t += h;
  }
  return resultingShadowColor ;
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
// to render any shape we can write an SDF for — including combinations,
// smooth blends, and fractals.
// =============================================================================
const int   MAX_STEPS = 120;   // max iterations before giving up
const float MAX_DIST  = 100.0; // ray is considered "missed" beyond this distance
const float SURF_DIST = 0.001; // close enough to the surface to count as a hit

float raymarch(vec3 ro, vec3 rd) {
    float dist = 0.0; // total distance travelled along the ray so far

    for (int i = 0; i < MAX_STEPS; i++) {
        vec3  p = ro + rd * dist;        // current position along the ray
        float d = sdScene(p);           // nearest surface distance from here
        dist   += d;                     // safe to advance by exactly d
        if (d < SURF_DIST || dist > MAX_DIST) break;
    }

    return dist; // if dist >= MAX_DIST, the ray missed everything
}


// =============================================================================
// SURFACE NORMALS VIA CENTRAL DIFFERENCES
//
// To light a surface we need its outward-facing normal vector at the hit point.
// For analytic shapes we could compute this mathematically, but there's a
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
        sdScene(p + vec3(e,0,0)) - sdScene(p - vec3(e,0,0)), // dF/dx
        sdScene(p + vec3(0,e,0)) - sdScene(p - vec3(0,e,0)), // dF/dy
        sdScene(p + vec3(0,0,e)) - sdScene(p - vec3(0,0,e))  // dF/dz
    ));
}


// Tweaked Cosine color palette function from Inigo Quilez
vec3 getColor(float amount) {
  vec3 color = vec3(0.3, 0.5, 0.9) +vec3(0.9, 0.4, 0.2) * cos(6.2831 * (vec3(0.30, 0.20, 0.20) + amount * vec3(1.0)));
  return color * amount;
}


// =============================================================================
// MAIN — builds the ray for this pixel, marches it, then shades the result
// =============================================================================
void main() {
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
    vec2 uv = frag_uv * vec2(aspect, 1.0);     // uv.x now spans ~[-1.33, 1.33]

    // -------------------------------------------------------------------------
    // Step 2: Build the camera basis vectors for a "look-at" setup.
    //
    // We want every ray to originate at the camera and pass through the scene.
    // To do that we construct three orthogonal vectors that describe the
    // camera's orientation:
    //
    //   fwd   — the direction the camera is facing (toward the target)
    //   right — points to the camera's right (perpendicular to fwd and world-up)
    //           note that world-up and fwd are not perpendicular if the camera
    //           and its target not aligned parallel to the xz-plane (y is up).
    //           Thus we also need to compute the up vector.
    //   up    — points upward relative to the camera's view
    //
    // Recall that the cross product of two vectors gives their orthogonal
    // counterpart following the right hand rule. This works for any camera
    // position.
    //
    // The only degenerate case is when the camera is directly above or below
    // the target (fwd is parallel to world-up), which makes the cross product
    // zero. In this case, we compute right using the cross product of fwd and
    // world-fwd (positive z), which always points in the positive x direction.
    // 
    // This formula discards camera roll. To account for that, we need to rotate
    // the fwd, right, and up basis vectors by the camera's rotation matrix.
    // -------------------------------------------------------------------------
    vec3 ro     = u_camera_pos;               // ray origin = camera position
    vec3 target = u_camera_tgt;               // point the camera looks at
    vec3 fwd    = normalize(target - ro);     // forward: camera → target
    
    // Handle the case where fwd is parallel world-up or world-down.
    vec3 right = vec3(0);
    if (fwd.y == -1 || fwd.y == 1) {
        right = normalize(cross(fwd, vec3(0,0,1))); // right: perpendicular to fwd and world-fwd
    } else {
        right = normalize(cross(fwd, vec3(0,1,0))); // right: perpendicular to fwd and world-up
    }
    vec3 up = cross(right, fwd);          // up: completes the right-handed basis

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
        vec3 light_pos = vec3(0, -5.0, 0);
        vec3 light_dir = normalize(light_pos - p); // arbitrary directional light

        float diffuse  = max(dot(normal, light_dir), 0.0);
        float ambient  = 0.1; // small constant so shadowed areas aren't pure black
        float shadows = softShadows(p, light_dir, 0.1, 5.0, 64.0);

        vec3 color = vec3(1.0, 1.0, 1.0) * getColor(ambient + diffuse * shadows);
        //vec3 sphereColor = vec3(0.1, 0.5, 1.0); // base blue color of the sphere
        //vec3 color = sphereColor * (ambient + diffuse * shadows);
        FragColor = vec4(color, 0);
    } else {
        // Discard the shader output if we didn't intersect the SDF.
        //
        // Some other things we could do for more sophisticated miss-handling:
        // 
        // # Write gl_FragDepth on hits
        // When a ray hits, we can compute the actual 3D hit position and write
        // a proper depth value to gl_FragDepth. That lets the GPU's depth test
        // work normally — rasterized objects closer to the camera will
        // naturally occlude the SDF surface, and the SDF surface will occlude
        // things behind it. Without this, the SDF ignores depth entirely.
        //
        // # Don't use a fullscreen quad
        // Instead of covering the whole screen, we render the SDF onto a
        // bounding volume (a box or sphere that tightly wraps the SDF scene).
        // The fragment shader only runs on pixels covered by that volume, so
        // the rest of the screen is untouched. This is more complex but the
        // most physically correct approach — it also naturally clips rays that
        // couldn't possibly hit the SDF object.
        discard;
    }
}
