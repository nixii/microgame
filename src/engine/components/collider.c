
#include "microgame/engine/components/collider.h"
#include <stdlib.h>

/*
 * region collider functions
*/

// create a new region collider
collider collider_new(vec3 size) {
    return (collider){
        .size = size,
        .centered = 1,
        .onCollision = NULL
    };
}

// get offset of the side
float collider_get_offset(collider *c, collider_side s) {
    switch (s) {
        case COLLIDER_SIDE_NORTH:
            return c->size.z / 2;
        case COLLIDER_SIDE_SOUTH:
            return c->size.z / -2;
        case COLLIDER_SIDE_EAST:
            return c->size.x / 2;
        case COLLIDER_SIDE_WEST:
            return c->size.x / -2;
        case COLLIDER_SIDE_TOP:
            return c->size.y / 2;
        case COLLIDER_SIDE_BOTTOM:
            return c->size.y / -2;
    }
    return 0;
}

// destroy a region colldier
void collider_destroy(collider *_) {}