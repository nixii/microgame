
#include "microgame/util/math.h"
#include <math.h>

/*******************
 * vec3 functions
*/

// create a new vec3 from floats
vec3 vec3_new(float x, float y, float z) {
    return (vec3){ .x = x, .y = y, .z = z };
}

// a new vec3 of <0, 0, 0>
vec3 vec3_zero() {
    return vec3_new(0, 0, 0);
}

// add two vec3s together
vec3 vec3_add(vec3 a, vec3 b) {
    return vec3_new(a.x + b.x, a.y + b.y, a.z + b.z);
}

// subtract two vec3s
vec3 vec3_sub(vec3 a, vec3 b) {
    return vec3_new(a.x - b.x, a.y - b.y, a.z - b.z);
}

// scale a vec3
vec3 vec3_mul(vec3 a, float b) {
    return vec3_new(a.x * b, a.y * b, a.z * b);
}

// shrink a vec3
vec3 vec3_div(vec3 a, float b) {
    return vec3_new(a.x / b, a.y / b, a.z / b);
}

// rotate a vec3 on the x axis
vec3 vec3_rotX(vec3 v, float a) {
    float c = cosf(a);
    float s = sinf(a);
    return vec3_new(v.x, v.y * c - v.z * s, v.y * s + v.z * c);
}

// rotate a vec3 on the y axis
vec3 vec3_rotY(vec3 v, float a) {
    float c = cosf(a);
    float s = sinf(a);
    return vec3_new(v.x * c + v.z * s, v.y, -v.x * s + v.z * c);
}

// rotate a vec3 on the z axis
vec3 vec3_rotZ(vec3 v, float a) {
    float c = cosf(a);
    float s = sinf(a);
    return vec3_new(v.x * c - v.y * s, v.x * s + v.y * c, v.z);
}

// rotate every axis
vec3 vec3_rot(vec3 v, vec3 a) {
    return vec3_rotZ(vec3_rotY(vec3_rotX(v, a.x), a.y), a.z);
}



/*******************
 * vec2 functions
*/

// create a new vec2 from floats
vec2 vec2_new(float x, float y) {
    return (vec2){ .x = x, .y = y };
}

// create a new 0-initialized vec2
vec2 vec2_zero() {
    return vec2_new(0, 0);
}

// add two vec2s
vec2 vec2_add(vec2 a, vec2 b) {
    return vec2_new(a.x + b.x, a.y + b.y);
}

// subtract two vec2s
vec2 vec2_sub(vec2 a, vec2 b) {
    return vec2_new(a.x - b.x, a.y - b.y);
}

// multiply two vec2s
vec2 vec2_mul(vec2 a, float b) {
    return vec2_new(a.x * b, a.y * b);
}

// divide two vec2s
vec2 vec2_div(vec2 a, float b) {
    return vec2_new(a.x / b, a.y / b);
}

/*******************
 * mat4 functions
*/

// create an identity matrix
mat4 mat4_identity() {
    return (mat4) {
        .m = {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
        }
    };
}

// create a translation matrix
mat4 mat4_translation(vec3 t) {
    mat4 m = mat4_identity();
    m.m[0][3] = t.x;
    m.m[1][3] = t.y;
    m.m[2][3] = t.z;
    return m;
}

// create a scale matrix
mat4 mat4_scale(vec3 s) {
    mat4 m = mat4_identity();
    m.m[0][0] = s.x;
    m.m[1][1] = s.y;
    m.m[2][2] = s.z;
    return m;
}

// a x rotation matrix
mat4 mat4_rotationX(float a) {
    float c = cosf(a);
    float s = sinf(a);

    return (mat4){{
        {1, 0,  0, 0},
        {0, c, -s, 0},
        {0, s,  c, 0},
        {0, 0,  0, 1}
    }};
}

// a y rotation matrix
mat4 mat4_rotationY(float a) {
    float c = cosf(a);
    float s = sinf(a);

    return (mat4){{
        { c, 0, s, 0},
        { 0, 1, 0, 0},
        {-s, 0, c, 0},
        { 0, 0, 0, 1}
    }};
}

// a z rotation matriz
mat4 mat4_rotationZ(float a) {
    float c = cosf(a);
    float s = sinf(a);

    return (mat4){{
        { c, -s, 0, 0},
        { s,  c, 0, 0},
        { 0,  0, 1, 0},
        { 0,  0, 0, 1}
    }};
}

// multiply matrix
mat4 mat4_mul(mat4 a, mat4 b) {
    mat4 r;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            r.m[i][j] =
                a.m[i][0] * b.m[0][j] +
                a.m[i][1] * b.m[1][j] +
                a.m[i][2] * b.m[2][j] +
                a.m[i][3] * b.m[3][j];
        }
    }
    return r;
}

// all the matrix operations into a model matrix
mat4 mat3_model(vec3 t, vec3 r, vec3 s) {
    return mat4_mul(
        mat4_translation(t),
        mat4_mul(
            mat4_rotationY(r.y),
            mat4_mul(
                mat4_rotationX(r.x),
                mat4_mul(
                    mat4_rotationZ(r.z),
                    mat4_scale(s)
                )
            )
        )
    );
}