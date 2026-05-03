
#ifndef MG_COMPONENT_COLLIDER_H
#define MG_COMPONENT_COLLIDER_H

#include "microgame/engine/entity.h"
#include "microgame/util/math.h"

// the collider's held info
typedef struct collider {
    vec3 size;
    int centered;
    void (*onCollision)(entity, entity);
    int collided;
} collider;

/*
 * region collider functions
*/

// create a new region collider
collider collider_new(vec3 size);

// destroy a region colldier
void collider_destroy(collider *c);

#endif // MG_COMPONENT_COLLIDER_H