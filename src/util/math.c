
#include "microgame/util/math.h"
#include <math.h>
#include <stdio.h>

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
mat4 mat4_model(vec3 t, vec3 r, vec3 s) {
    mat4 matTranslation = mat4_translation(t);
    mat4 matRotY = mat4_rotationY(r.y);
    mat4 matRotX = mat4_rotationX(r.x);
    mat4 matRotZ = mat4_rotationZ(r.z);
    mat4 matScale = mat4_scale(s);

    return mat4_mul(matTranslation, mat4_mul(matRotY, mat4_mul(matRotX, mat4_mul(matRotZ, matScale))));
}

// inverse a matrix
mat4 mat4_fast_inverse(mat4 m) {
    vec3 t = vec3_new(m.m[0][3], m.m[1][3], m.m[2][3]);

    // scale
    vec3 sx = vec3_new(m.m[0][0], m.m[1][0], m.m[2][0]);
    vec3 sy = vec3_new(m.m[0][1], m.m[1][1], m.m[2][1]);
    vec3 sz = vec3_new(m.m[0][2], m.m[1][2], m.m[2][2]);

    // lengths for rotation
    float lx = sqrtf(sx.x*sx.x + sx.y*sx.y + sx.z*sx.z);
    float ly = sqrtf(sy.x*sy.x + sy.y*sy.y + sy.z*sy.z);
    float lz = sqrtf(sz.x*sz.x + sz.y*sz.y + sz.z*sz.z);

    // normalize for rotation
    sx = vec3_div(sx, lx);
    sy = vec3_div(sy, ly);
    sz = vec3_div(sz, lz);

    // rot inverse
    mat4 rInv = mat4_identity();
    rInv.m[0][0] = sx.x; 
    rInv.m[0][1] = sx.y; 
    rInv.m[0][2] = sx.z;
    rInv.m[1][0] = sy.x; 
    rInv.m[1][1] = sy.y; 
    rInv.m[1][2] = sy.z;
    rInv.m[2][0] = sz.x; 
    rInv.m[2][1] = sz.y; 
    rInv.m[2][2] = sz.z;

    // scale inverse
    mat4 sInv = mat4_scale((vec3){
        1.0 / lx,
        1.0 / ly,
        1.0 / lz
    });

    // translation inverse
    mat4 tInv = mat4_translation(vec3_new(-t.x, -t.y, -t.z));

    // combine
    return mat4_mul(sInv, mat4_mul(rInv, tInv));
}

// simply print it out
void mat4_display(mat4 m) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%f ", m.m[i][j]);
        }
        printf("\n");
    }
}
