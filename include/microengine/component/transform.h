
#ifndef MICROENGINE_TRANSFORM_H
#define MICROENGINE_TRANSFORM_H

#include "microengine/math.h"

typedef struct me_transform {
    me_vec3 pos;
    me_vec3 rot;
    me_vec3 scale;
} me_transform;

// create a new transform
me_transform me_transform_new(me_vec3 pos, me_vec3 rot, me_vec3 scale);

// create an empty transform
me_transform me_transform_blank();

// destroy a transform (does nothing)
void me_transform_destroy(me_transform *transform);

#ifndef MICROENGINE_NOSTRIP
    #define transform me_transform
    #define transform_new me_transform_new
    #define transform_blank me_transform_blank
    #define transform_destroy me_transform_destroy
#endif

#endif // MICROENGINE_TRANSFORM_H