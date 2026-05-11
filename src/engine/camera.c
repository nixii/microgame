
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
    if (p.z <= 0)
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

// FIXME: I think this function is the issue.
// intersenct a triangle with the near clipping plane
static vec3 intersect_near(vec3 inside, vec3 outside) {
    float t = (NEAR_CLIP - inside.z) / (outside.z - inside.z);

    vec3 r;
    r.x = inside.x + (outside.x - inside.x) * t;
    printf("in z: %f\n", inside.z);
    printf("out z: %f\n", outside.z);
    printf("t: %f\n", t);
    printf("rx: %f\n\n", r.x);
    r.y = inside.y + (outside.y - inside.y) * t;
    r.z = NEAR_CLIP;
    return r;
}

// FIXME: this function does too much. split it up.
// translate a triangle and maybe split it
camera_translation_result camera_translate_triangle(camera *c, vec3 lv1, vec3 lv2, vec3 lv3) {

    camera_translation_result result;
    result.numTriangles = 0;

    // get the normal of the vertex
    vec3 normal = NORMALIZE(
        vec3_cross(SUB(lv2, lv1), SUB(lv3, lv1))
    );

    // transform the vertices to camera space
    vec3 v1 = camera_transform(c, lv1);
    vec3 v2 = camera_transform(c, lv2);
    vec3 v3 = camera_transform(c, lv3);

    // get the normal of the vertex in camera space
    vec3 cameraNormal = NORMALIZE(
        CROSS(SUB(v2, v1), SUB(v3, v1))
    );
    
    // backface cull it
    // if (DOT(cameraNormal, v1) >= 0)
    //     return result;

    // store in array for easy use
    vec3 vs[3] = {lv1, lv2, lv3};
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
        int i1i = insideIdx[0];
        int i2i = insideIdx[1];

        vec3 o  = v[oi];
        vec3 i1 = v[i1i];
        vec3 i2 = v[i2i];

        vec3 ni1 = intersect_near(i1, o);
        vec3 ni2 = intersect_near(i2, o);

        result.triangles[0] = (triangle){
            .a = ni1, .b = i1, .c = i2, .normal = normal
        };
        result.triangles[1] = (triangle){
            .a = ni2, .b = i2, .c = ni1, .normal = normal
        };
        result.numTriangles = 2;
        return result;
    }

    return result;
}