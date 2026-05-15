
#ifndef MG_UTIL_H
#define MG_UTIL_H

#include "microgame/util/vec3.h"
#include "microgame/util/vec2.h"

/*
 * special macros to make life easier
*/
#define ADD(a, b) _Generic((a), \
    vec2: vec2_add, \
    vec3: vec3_add)(a, b)
#define SUB(a, b) _Generic((a), \
    vec2: vec2_sub, \
    vec3: vec3_sub)(a, b)
#define MUL(a, b) _Generic((a), \
    vec2: vec2_mul, \
    vec3: vec3_mul)(a, b)
#define MUL_COMPONENTS(a, b) _Generic((a), \
    vec2: vec2_mul_components, \
    vec3: vec3_mul_components)(a, b)
#define DIV(a, b) _Generic((a), \
    vec2: vec2_div, \
    vec3: vec3_div)(a, b)
#define DOT(a, b) _Generic((a), \
    vec2: vec2_dot, \
    vec3: vec3_dot)(a, b)
#define LENGTH(a) _Generic((a), \
    vec2: vec2_length, \
    vec3: vec3_length)(a)
#define NORMALIZE(a) _Generic((a), \
    vec2: vec2_normalize, \
    vec3: vec3_normalize)(a)
#define CROSS(a, b) vec3_cross((a), (b))

// mat4 type
typedef struct mat4 {
    float m[4][4];
} mat4;

// mat4 functions
mat4 mat4_identity();
mat4 mat4_translation(vec3 t);
mat4 mat4_scale(vec3 s);
mat4 mat4_rotationX(float a);
mat4 mat4_rotationY(float a);
mat4 mat4_rotationZ(float a);
mat4 mat4_mul(mat4 a, mat4 b);
mat4 mat4_model(vec3 t, vec3 r, vec3 s);
mat4 mat4_fast_inverse(mat4 m);
void mat4_display(mat4 m);

// general math
static inline float minf(float a, float b) {
    return a <= b ? a : b;
}
static inline float maxf(float a, float b) {
    return a >= b ? a : b;
}
static inline float clampf(float a, float min, float max) {
    return minf(max, maxf(min, a));
}

#endif // MG_UTIL_H