
#ifndef MG_COMPONENT_REGIONCOLLIDER_H
#define MG_COMPONENT_REGIONCOLLIDER_H

#include "microgame/util/math.h"

typedef struct region_collider {
    vec3 size;
    int centered
} region_collider;

/*
 * region collider functions
*/

// create a new region collider
region_collider region_collider_new(vec3 size, int centered);

// destroy a region colldier
void region_collider_destroy(region_collider *c);

#endif // MG_COMPONENT_REGIONCOLLIDER_H