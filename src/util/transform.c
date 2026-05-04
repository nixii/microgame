
#include "microgame/util/transform.h"
#include <math.h>

/************************
 * transform functions
*/

// create a new transform
transform transform_new(vec3 pos, vec3 rot, vec3 scale) {
    return (transform){ .pos = pos, .rot = rot, .scale = scale };
}

// create a new empty transform (pos/rot 0, scale 1)
transform transform_blank() {
    return transform_new(vec3_zero(), vec3_zero(), vec3_new(1, 1, 1));
}

// make a transform from a mat4
transform transform_from_mat(mat4 m) {
    transform t;

    // get the position
    t.pos = vec3_new(m.m[0][3], m.m[1][3], m.m[2][3]);

    // set the scale
    vec3 c0 = vec3_new(m.m[0][0], m.m[1][0], m.m[2][0]);
    vec3 c1 = vec3_new(m.m[0][1], m.m[1][1], m.m[2][1]);
    vec3 c2 = vec3_new(m.m[0][2], m.m[1][2], m.m[2][2]);
    t.scale = vec3_new(vec3_len(c0), vec3_len(c1), vec3_len(c2));

    // rotation scaling
    c0 = vec3_div(c0, t.scale.x);
    c1 = vec3_div(c1, t.scale.y);
    c2 = vec3_div(c2, t.scale.z);
    float r00 = c0.x, r01 = c1.x, r02 = c2.x;
    float r10 = c0.y, r11 = c1.y, r12 = c2.y;
    float r20 = c0.z, r21 = c1.z, r22 = c2.z;

    // get rotation
    float pitch = asinf(-r21);
    float yaw, roll;
    if (fabsf(r21) < 0.9999f) {
        yaw  = atan2f(r20, r22);
        roll = atan2f(r01, r11);
    } else {
        yaw  = atan2f(-r02, r00);
        roll = 0.0f;
    }

    // all done
    t.rot = vec3_new(pitch, yaw, roll);
    return t;
}