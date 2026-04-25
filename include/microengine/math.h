
#ifndef MICROENGINE_VEC3_H
#define MICROENGINE_VEC3_H

typedef struct me_vec3 {
    float x;
    float y;
    float z;
} me_vec3;

// create a new vec3
me_vec3 me_vec3_new(float x, float y, float z);

// get the length
float me_vec3_len(me_vec3 *v);

// add two
me_vec3 me_vec3_add(me_vec3 *a, me_vec3 *b);

// sub two
me_vec3 me_vec3_sub(me_vec3 *a, me_vec3 *b);

// multiply
me_vec3 me_vec3_mul(me_vec3 *a, float scalar);

// divide
me_vec3 me_vec3_div(me_vec3 *a, float scalar);


// strip the prefix by default
#ifndef MICROENGINE_NOSTRIP
    #define vec3 me_vec3
    #define vec3_new me_vec3_new
    #define vec3_len me_vec3_len
    #define vec3_add me_vec3_add
    #define vec3_sub me_vec3_sub
    #define vec3_mul me_vec3_mul
    #define vec3_div me_vec3_div
#endif

#endif // MICROENGINE_VEC3_H