
#include "microgame/engine/camera.h"
#include "microgame/util/triangle.h"

camera camera_new() {
    return (camera) {
        .fov = 3.14 / 2,
        .transform = transform_blank()
    };
}

vec3 camera_transform(camera *c, vec3 p) {
    p = vec3_sub(p, c->transform.pos);
    p = vec3_rot(p, vec3_mul(c->transform.rot, -1));
    return p;
}

vec2 camera_project_point(camera *c, vec3 p, int width, int height) {

    // avoid divide-by-zero
    if (p.z <= 0) return vec2_new(-1, -1);

    float f = c->fov / p.z;

    float x = (p.x * f) / ((float)width / height);
    float y = p.y * f;

    vec2 out;

    out.x = (x + 1.0f) * 0.5f * width;
    out.y = (1.0f - (y + 1.0f) * 0.5f) * height;

    return out;
}

// translate a triangle to camera location
camera_translation_result camera_translate_triangle(camera *c, vec3 v1, vec3 v2, vec3 v3) {

    camera_translation_result result;
    result.numTriangles = 1;

    // define the triangle
    triangle tri;
    tri.a = camera_transform(c, v1);
    tri.b = camera_transform(c, v2);
    tri.c = camera_transform(c, v3);
    tri.normal = vec3_normal(vec3_cross(vec3_sub(v2, v1), vec3_sub(v3, v2)));
    tri.depth = (tri.a.z + tri.b.z + tri.c.z) / 3.0;

    // add the triangle
    result.triangles[0] = tri;
    
    // return the result
    return result;
}