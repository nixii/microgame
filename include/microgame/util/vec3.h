
/**********************
 * vec3 Documentation *
 **********************/

/*********
 * Types
*/

// struct vec3
/*
     ~ float x
     ~ float y
     ~ float z
*/

/*************
 * Functions
*/

////// CONSTRUCTOES
// vec3 vec3_new(float x, float y, float z)
// vec3 vec3_zero()
// vec3 vec3_x()
// vec3 vec3_y()
// vec3 vec3_z()

////// ARITHMETIC
// vec3 vec3_add(vec3 a, vec3 b)
// vec3 vec3_sub(vec3 a, vec3 b)
// vec3 vec3_mul(vec3 a, float scalar)
// vec3 vec3_div(vec3 a, float scalar)

////// GEOMETRY
// float vec3_length(vec3 a)
// vec3 vec3_normalize(vec3 a)
// float vec3_dot(vec3 a, vec3 b)
// vec3 vec3_cross(vec3 a, vec3 b)

#ifndef MG_VEC3_H
#define MG_VEC3_H

#include <math.h>

// simple vec3 implementation
typedef struct vec3 {
    float x, y, z;
} vec3;


/*
 * vec3 macros
*/

// debug formatting.
#define VEC3_FMT "(vec3<%.3f  %.3f  %.3f>)"
#define VEC3_ARGS(a) (a).x, (a).y, (a).z


/*
 * handle vec3s
*/

// construct a new vec3
vec3 vec3_new(float x, float y, float z);

// create a new empty vec3
vec3 vec3_zero();

// axes
vec3 vec3_x();
vec3 vec3_y();
vec3 vec3_z();

// add two vec3s
// creates a new vec3
vec3 vec3_add(vec3 a, vec3 b);

// subtract two vec3s
// creates a new vec3
vec3 vec3_sub(vec3 a, vec3 b);

// scale a vec3
// creates a new vec3
vec3 vec3_mul(vec3 a, float scalar);

// multiply components
// creates a new vec3
vec3 vec3_mul_components(vec3 a, vec3 b);

// divide a vec3
// creates a new vec3
vec3 vec3_div(vec3 a, float scalar);

// get the length of a vec3
float vec3_length(vec3 a);

// normalize a vec3
// creates a new vec3
vec3 vec3_normalize(vec3 a);

// dot a vec3
float vec3_dot(vec3 a, vec3 b);

// cross a vec3
vec3 vec3_cross(vec3 a, vec3 b);

// rotate a vec3 on the axes
vec3 vec3_rot_x(vec3 a, float theta);
vec3 vec3_rot_y(vec3 a, float theta);
vec3 vec3_rot_z(vec3 a, float theta);
vec3 vec3_rot(vec3 a, vec3 rotation);

#endif // MG_VEC3_H