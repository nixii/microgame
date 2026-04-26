
#include "microgame/engine/camera.h"

camera camera_new() {
    return (camera) {
        .fov = 3.14 / 2,
        .transform = transform_blank()
    };
}

vec2 camera_project_point(camera *c, vec3 p, int width, int height) {
    
    // transform!
    p = vec3_sub(p, c->transform.pos);
    p = vec3_rot(p, vec3_mul(c->transform.rot, -1));

    // avoid divide-by-zero
    if (p.z <= 0) return vec2_new(-1, -1);

    float f = c->fov / p.z;

    float x = p.x * f;
    float y = p.y * f;

    vec2 out;

    out.x = (x + 1.0f) * 0.5f * width;
    out.y = (1.0f - (y + 1.0f) * 0.5f) * height;

    return out;
}