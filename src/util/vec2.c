
#include "microgame/util/vec2.h"

/*
 * vec2 functions
*/

// create a new basic vec2
vec2 vec2_new(float x, float y) { return (vec2){.x=x, .y=y}; }

// empty vec2
vec2 vec2_zero() { return vec2_new(0, 0); }

// axes
vec2 vec2_x() { return vec2_new(1, 0); }
vec2 vec2_y() { return vec2_new(0, 1); }

// arithmetic
vec2 vec2_add(vec2 a, vec2 b) { return vec2_new(a.x + b.x, a.y + b.y); }
vec2 vec2_sub(vec2 a, vec2 b) { return vec2_new(a.x - b.x, a.y - b.y); }
vec2 vec2_mul(vec2 a, float scalar) { return vec2_new(a.x * scalar, a.y * scalar); }
vec2 vec2_mul_components(vec2 a, vec2 b) { return vec2_new(a.x * b.x, a.y * b.y); }
vec2 vec2_div(vec2 a, float scalar) { return vec2_new(a.x / scalar, a.y / scalar); }

// geometry
float vec2_dot(vec2 a, vec2 b) { return a.x * b.x + a.y * b.y; }
float vec2_length(vec2 a) { return sqrtf(a.x * a.x + a.y * a.y); }
vec2 vec2_normalize(vec2 a) { return vec2_div(a, vec2_length(a)); }