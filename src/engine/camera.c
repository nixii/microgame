
#include "microgame/engine/camera.h"
#include "microgame/util/triangle.h"
#include <stdio.h>

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

projection_result camera_project_point(camera *c, vec3 p, int width, int height) {

    // avoid divide-by-zero
    if (p.z <= NEAR_CLIP)
        return (projection_result){ .failure = 1 };

    float f = 1.0f / tanf(c->fov * 0.5f);
    float x = (p.x * f) / p.z / ((float)width / height);
    float y = (p.y * f) / p.z;

    vec3 out;

    out.x = (x + 1.0f) * 0.5f * width;
    out.y = (1.0f - (y + 1.0f) * 0.5f) * height;
    out.z = p.z;

    return (projection_result){ .failure = 0, .vec = out };
}

// intersenct a triangle with the near clipping plane
static vec3 intersect_near_plane(vec3 a, vec3 b) {
    float t = (a.z - NEAR_CLIP) / (a.z - b.z);
    return (vec3){
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y),
        NEAR_CLIP
    };
} 

// translate a triangle to camera space and near clip ittttttttttt
camera_translation_result camera_translate_triangle(camera *c, vec3 world1, vec3 world2, vec3 world3) {
    
    // store the final result
    camera_translation_result result;
    result.numTriangles = 0;

    // the input triangles
    vec3 input[4] = {
        camera_transform(c, world1),
        camera_transform(c, world2),
        camera_transform(c, world3),
    };
    int n_input = 3;

    // get the normal of the vertex in camera space
    vec3 cameraNormal = NORMALIZE(
        CROSS(SUB(input[1], input[0]), SUB(input[2], input[0]))
    );
    
    // backface cull it
    if (DOT(cameraNormal, input[0]) >= 0)
        return result;
     
    // the regular normal
    vec3 normal = NORMALIZE(
        CROSS(SUB(world2, world1), SUB(world3, world1))
    );

    // the output triangles from the clip detection
    vec3 output[4];
    int  n_output = 0;

    // detcet clips
    for (int i = 0; i < n_input; i++) {
        vec3 cur  = input[i];
        vec3 prev = input[(i + n_input - 1) % n_input];
        int  cur_in  = cur.z  > NEAR_CLIP + NEAR_CLIP_EPSILON;
        int  prev_in = prev.z > NEAR_CLIP + NEAR_CLIP_EPSILON;

        if (prev_in && !cur_in) {
            output[n_output++] = intersect_near_plane(prev, cur);
        } else if (!prev_in && cur_in) {
            output[n_output++] = intersect_near_plane(prev, cur);
            output[n_output++] = cur;
        } else if (cur_in) {
            output[n_output++] = cur;
        }
    }

    // render all the triangles on their clipped results
    for (int i = 1; i + 1 < n_output; i++) {
        result.triangles[result.numTriangles++] = (triangle){
            .a = output[0],
            .b = output[i],
            .c = output[i + 1],
            .normal = normal
        };
    }

    // return the final result yay!
    return result;
}