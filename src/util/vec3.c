
#include "microgame/util/vec3.h"

/*
 * vec3 funnctions
*/

// construct a new vec3
vec3 vec3_new(float x, float y, float z) {
    return (vec3){.x=x, .y=y, .z=z};
}

// create a new empty vec3
vec3 vec3_zero() {
    return vec3_new(0, 0, 0);
}

// axes
vec3 vec3_x() { return vec3_new(1, 0, 0); }
vec3 vec3_y() { return vec3_new(0, 1, 0); }
vec3 vec3_z() { return vec3_new(0, 0, 1); }

// add two vec3s
// creates a new vec3
vec3 vec3_add(vec3 a, vec3 b) {
    return vec3_new(a.x + b.x, a.y + b.y, a.z + b.z);
}

// subtract two vec3s
// creates a new vec3
vec3 vec3_sub(vec3 a, vec3 b) {
    return vec3_new(a.x - b.x, a.y - b.y, a.z - b.z);
}

// scale a vec3
// creates a new vec3
vec3 vec3_mul(vec3 a, float scalar) {
    return vec3_new(a.x * scalar, a.y * scalar, a.z * scalar);
}

// divide a vec3
// creates a new vec3
vec3 vec3_div(vec3 a, float scalar) {
    return vec3_new(a.x / scalar, a.y / scalar, a.z / scalar);
}

// get the length of a vec3
float vec3_length(vec3 a) { return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z); }

// normalize a vec3
// creates a new vec3
vec3 vec3_normalize(vec3 a) { return vec3_div(a, vec3_length(a)); }

// dot a vec3
float vec3_dot(vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

// cross a vec3
vec3 vec3_cross(vec3 a, vec3 b) {
    return vec3_new(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
};

// rotate a vec3 on the axes
vec3 vec3_rot_x(vec3 a, float theta) {
    float c = cosf(theta), s = sinf(theta);
    return vec3_new(a.x, a.y*c - a.z*s, a.y*s + a.z*c);
}
vec3 vec3_rot_y(vec3 a, float theta) {
    float c = cosf(theta), s = sinf(theta);
    return vec3_new(a.x*c + a.z*s, a.y, -a.x*s + a.z*c);
}
vec3 vec3_rot_z(vec3 a, float theta) {
    float c = cosf(theta), s = sinf(theta);
    return vec3_new(a.x*c - a.y*s, a.x*s + a.y*c, a.z);
}
vec3 vec3_rot(vec3 a, vec3 rotation) {
    return vec3_rot_z(vec3_rot_x(vec3_rot_y(a, rotation.y), rotation.x), rotation.z);
}

// turn rotation into a unit vector
vec3 vec3_get_axis(vec3 a) {
    float x = cosf(a.y) * cosf(a.x);
    float y = sinf(a.y) * cosf(a.x);
    float z = sinf(a.x);
    return vec3_new(x, y, z);
}