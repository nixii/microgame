
#ifndef MG_COMPONENT_COLLIDER_H
#define MG_COMPONENT_COLLIDER_H

#include "microgame/util/transform.h"
#include "microgame/engine/entity.h"
#include "microgame/util/math.h"

// forward-declare a scene
typedef struct scene scene;

// different sides
typedef enum collider_side {
    COLLIDER_SIDE_NORTH,  // +z
    COLLIDER_SIDE_SOUTH,  // -z
    COLLIDER_SIDE_EAST,   // +x
    COLLIDER_SIDE_WEST,   // -x
    COLLIDER_SIDE_TOP,    // +y
    COLLIDER_SIDE_BOTTOM, // -y
} collider_side;

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

// get offset of the side
float collider_get_offset(collider *c, transform globalTransform, collider_side s);

// does a collider collide
int collider_collides(scene *s, entity e1, entity e2);

// destroy a region colldier
void collider_destroy(collider *c);

#endif // MG_COMPONENT_COLLIDER_H