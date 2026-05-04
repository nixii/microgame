
#ifndef MG_TRANSFORM_H
#define MG_TRANSFORM_H

#include "microgame/util/math.h"

// transform type
typedef struct transform {
    vec3 pos;
    vec3 rot;
    vec3 scale;
} transform;

// transform functions
transform transform_new(vec3 pos, vec3 rot, vec3 scale);
transform transform_from_mat(mat4 m);
transform transform_blank();

#endif // MG_TRANSFORM_H