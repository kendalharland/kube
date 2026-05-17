#version 330 core

in vec2 frag_uv;
out vec3 FragColor;

// Camera uniforms — set these from your C++ side
uniform vec3 u_camera_pos;       // e.g. vec3(0, 0, 4)
uniform vec2 u_resolution;       // viewport width, height in pixels
uniform float u_time;            // seconds elapsed, for animation (optional)

// --- SDF ---
float sdfSphere(vec3 p, float radius) {
    return length(p) - radius;
}

// --- Raymarcher ---
const int   MAX_STEPS = 100;
const float MAX_DIST  = 100.0;
const float SURF_DIST = 0.001;

float march(vec3 ro, vec3 rd) {
    float dist = 0.0;
    for (int i = 0; i < MAX_STEPS; i++) {
        vec3  p   = ro + rd * dist;
        float d   = sdfSphere(p, 1.0); // sphere at origin, radius 1
        dist     += d;
        if (d < SURF_DIST || dist > MAX_DIST) break;
    }
    return dist;
}

// --- Normal via central differences ---
vec3 calcNormal(vec3 p) {
    const float e = 0.001;
    return normalize(vec3(
        sdfSphere(p + vec3(e,0,0), 1.0) - sdfSphere(p - vec3(e,0,0), 1.0),
        sdfSphere(p + vec3(0,e,0), 1.0) - sdfSphere(p - vec3(0,e,0), 1.0),
        sdfSphere(p + vec3(0,0,e), 1.0) - sdfSphere(p - vec3(0,0,e), 1.0)
    ));
}

void main() {
    float aspect = u_resolution.x / u_resolution.y;
    vec2  uv     = frag_uv * vec2(aspect, 1.0);

    vec3 ro     = u_camera_pos;
    vec3 target = vec3(0.0);                         // always look at origin
    vec3 fwd    = normalize(target - ro);
    vec3 right  = normalize(cross(fwd, vec3(0,1,0)));
    vec3 up     = cross(right, fwd);
    vec3 rd     = normalize(fwd * 1.5 + right * uv.x + up * uv.y); // 1.5 ≈ 67° FOV

    float dist = march(ro, rd);

    if (dist < MAX_DIST) {
        vec3 p       = ro + rd * dist;
        vec3 normal  = calcNormal(p);

        // Simple diffuse + ambient lighting
        vec3 lightDir  = normalize(vec3(1.0, 2.0, 3.0));
        float diffuse  = max(dot(normal, lightDir), 0.0);
        float ambient  = 0.1;

        vec3 sphereColor = vec3(0.2, 0.5, 1.0); // blue-ish
        FragColor = sphereColor * (ambient + diffuse);
    } else {
        FragColor = vec3(0.05); // background
    }
}

