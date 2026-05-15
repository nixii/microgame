
#ifndef MG_VEC2_H
#define MG_VEC2_H

#include <math.h>

// a basic vec2 type
typedef struct vec2 {
    float x, y;
} vec2;


/*
 * vec2 macros
*/
#define VEC2_FMT "(vec2<%.3f  %.3f>)"
#define VEC2_ARGS(a) (a).x, (a).y


/*
 * vec2 functions
*/

// create a new basic vec2
vec2 vec2_new(float x, float y);

// empty vec2
vec2 vec2_zero();

// axes
vec2 vec2_x();
vec2 vec2_y();

// arithmetic
vec2 vec2_add(vec2 a, vec2 b);
vec2 vec2_sub(vec2 a, vec2 b);
vec2 vec2_mul(vec2 a, float scalar);
vec2 vec2_mul_components(vec2 a, vec2 b);
vec2 vec2_div(vec2 a, float scalar);

// geometry
float vec2_dot(vec2 a, vec2 b);
float vec2_length(vec2 a);
vec2 vec2_normalize(vec2 a);


#endif // MG_VEC2_H