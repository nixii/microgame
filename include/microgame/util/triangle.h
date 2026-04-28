
#ifndef MG_TRIANGLE_H
#define MG_TRIANGLE_H

#include "microgame/util/math.h"
#include "microgame/util/color.h"

typedef struct triangle {
    vec3 a, b, c, normal;
    color color;
} triangle;

int triangle_cmp(const void *a, const void *b);

#endif // MG_TRIANGLE_H