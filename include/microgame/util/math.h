
#ifndef MG_UTIL_H
#define MG_UTIL_H

// vec3 type
typedef struct vec3 {
    float x;
    float y;
    float z;
} vec3;

// vec3 functions
vec3 vec3_new(float x, float y, float z);
vec3 vec3_zero();
vec3 vec3_x();
vec3 vec3_y();
vec3 vec3_z();
vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_sub(vec3 a, vec3 b);
vec3 vec3_mul(vec3 a, float b);
vec3 vec3_div(vec3 a, float b);
vec3 vec3_rotX(vec3 v, float a);
vec3 vec3_rotY(vec3 v, float a);
vec3 vec3_rotZ(vec3 v, float a);
vec3 vec3_rot(vec3 v, vec3 a);
vec3 vec3_cross(vec3 a, vec3 b);
float vec3_dot(vec3 a, vec3 b);
float vec3_len(vec3 a);
vec3 vec3_normal(vec3 a);

// vec2 type
typedef struct vec2 {
    float x;
    float y;
} vec2;

// vec2 functions
vec2 vec2_new(float x, float y);
vec2 vec2_zero();
vec2 vec2_add(vec2 a, vec2 b);
vec2 vec2_sub(vec2 a, vec2 b);
vec2 vec2_mul(vec2 a, float b);
vec2 vec2_div(vec2 a, float b);

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

#endif // MG_UTIL_H