
#include <math.h>
#include "microengine/math.h"

// create a new vec3
me_vec3 me_vec3_new(float x, float y, float z)
{
    return (me_vec3){
        .x = x,
        .y = y,
        .z = z
    };
}

// get the length
float me_vec3_len(me_vec3 v)
{
    return sqrtf(
        v.x * v.x +
        v.y * v.y +
        v.z * v.z
    );
}

// add two
me_vec3 me_vec3_add(me_vec3 a, me_vec3 b)
{
    return (me_vec3){
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z
    };
}

// sub two
me_vec3 me_vec3_sub(me_vec3 a, me_vec3 b)
{
    return (me_vec3){
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z
    };
}

// multiply
me_vec3 me_vec3_mul(me_vec3 a, float scalar)
{
    return (me_vec3){
        .x = a.x * scalar,
        .y = a.y * scalar,
        .z = a.z * scalar
    };
}

// divide
me_vec3 me_vec3_div(me_vec3 a, float scalar)
{
    return (me_vec3){
        .x = a.x / scalar,
        .y = a.y / scalar,
        .z = a.z / scalar
    };
}

// rotate x, y, and z
me_vec3 me_vec3_rotateX(me_vec3 v, float a)
{
    float c = cosf(a);
    float s = sinf(a);
    return (me_vec3){
        v.x,
        v.y * c - v.z * s,
        v.z * s + v.z * c
    };
}
me_vec3 me_vec3_rotateY(me_vec3 v, float a)
{
    float c = cosf(a);
    float s = sinf(a);
    return (me_vec3){
        v.x * c + v.z * s,
        v.y,
        -v.x * s + v.z * c
    };
}
me_vec3 me_vec3_rotateZ(me_vec3 v, float a)
{
    float c = cosf(a);
    float s = sinf(a);
    return (me_vec3){
        v.x * c - v.y * s,
        v.x * s + v.y * c,
        v.z
    };
}

// rotate all 3
me_vec3 me_vec3_rotate(me_vec3 v, me_vec3 a)
{
    return me_vec3_rotateZ(me_vec3_rotateY(me_vec3_rotateX(v, a.x), a.y), a.z);
}