
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

vec3 camera_project_point(camera *c, vec3 p, int width, int height) {

    // avoid divide-by-zero
    if (p.z <= 0) return vec3_new(-1, -1, -1);

    float f = c->fov / p.z;

    float x = (p.x * f) / ((float)width / height);
    float y = p.y * f;

    vec3 out;

    out.x = (x + 1.0f) * 0.5f * width;
    out.y = (1.0f - (y + 1.0f) * 0.5f) * height;
    out.z = p.z;

    return out;
}

// intersenct a triangle with the near clipping plane
static vec3 intersect_near(vec3 inside, vec3 outside) {
    float t = (NEAR_CLIP - inside.z) / (outside.z - inside.z);

    vec3 r;
    r.x = inside.x + (outside.x - inside.x) * t;
    r.y = inside.y + (outside.y - inside.y) * t;
    r.z = NEAR_CLIP;
    return r;
}

// FIXME: this function does too much. split it up.
// translate a triangle and maybe split it
camera_translation_result camera_translate_triangle(camera *c, vec3 v1, vec3 v2, vec3 v3) {

    camera_translation_result result;
    result.numTriangles = 0;

    // get the normal of the vertex
    vec3 normal = NORMALIZE(
        vec3_cross(SUB(v2, v1), SUB(v3, v2))
    );

    // transform the vertices to camera space
    v1 = camera_transform(c, v1);
    v2 = camera_transform(c, v2);
    v3 = camera_transform(c, v3);

    // get the normal of the vertex in camera space
    vec3 cameraNormal = NORMALIZE(
        CROSS(SUB(v2, v1), SUB(v3, v2))
    );
    
    // backface cull it
    // FIXME: backface culling is not working correctly at certain camera angles.
    if (DOT(cameraNormal, v1) >= 0)
        return result;

    // store in array for easy use
    vec3 v[3] = {v1, v2, v3};

    // the amount of triangles inside or outside the view plane
    vec3 inside[3];
    vec3 outside[3];
    int insideIdx[3];
    int outsideIdx[3];
    int inCount = 0, outCount = 0;

    // load all the vertices and indices
    for (int i = 0; i < 3; i++) {
        if (v[i].z >= NEAR_CLIP) {
            inside[inCount] = v[i];
            insideIdx[inCount] = i;
            inCount++;
        } else {
            outside[outCount] = v[i];
            outsideIdx[outCount] = i;
            outCount++;
        }
    }

    // all outside
    if (inCount == 0) return result;

    // 0 outside
    if (outCount == 0) {
        triangle tri = {
            .a = inside[0], 
            .b = inside[1], 
            .c = inside[2], 
            .normal = normal
        };
        result.triangles[0] = tri;
        result.numTriangles = 1;
        return result;
    }

    // 2 outside
    if (inCount == 1 && outCount == 2) {

        // get points, preserving winding order
        int ii = insideIdx[0];
        int o1i = (ii + 1) % 3;
        int o2i = (ii + 2) % 3;

        vec3 i  = v[ii];
        vec3 o1 = v[o1i];
        vec3 o2 = v[o2i];

        result.triangles[0] = (triangle){
            .a = i,
            .b = intersect_near(i, o1),
            .c = intersect_near(i, o2),
            .normal = normal
        };
        result.numTriangles = 1;
        return result;
    }

    // 1 outside
    if (inCount == 2 && outCount == 1) {

        // get points IN THE WINDING ORDERRRRRRRRRRRR
        int oi = outsideIdx[0];
        int i1i = (oi + 1) % 3;
        int i2i = (oi + 2) % 3; 

        vec3 o  = v[oi];
        vec3 i1 = v[i1i];
        vec3 i2 = v[i2i];

        vec3 ni1 = intersect_near(i1, o);
        vec3 ni2 = intersect_near(i2, o);

        result.triangles[0] = (triangle){
            .a = i1, .b = i2, .c = ni1, .normal = normal
        };
        result.triangles[1] = (triangle){
            .a = i2, .b = ni2, .c = ni1, .normal = normal
        };
        result.numTriangles = 2;
        return result;
    }

    return result;
}