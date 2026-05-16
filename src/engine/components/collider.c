
#include "microgame/engine/components/collider.h"
#include "microgame/engine/scene.h"
#include <stdlib.h>

/*
 * region collider functions
*/

// create a new region collider
collider collider_new(vec3 size) {
    return (collider){
        .size = size,
        .centered = 1,
        .onCollision = NULL,
        .hitFloor = 0,
        .hitWall = 0,
        .hitCeiling = 0
    };
}

// get offset of the side
float collider_get_offset(collider *c, transform globalTransform, collider_side s) {
    switch (s) {
        case COLLIDER_SIDE_NORTH:
            return (c->size.z / 2) * globalTransform.scale.z;
        case COLLIDER_SIDE_SOUTH:
            return (c->size.z / -2) * globalTransform.scale.z;
        case COLLIDER_SIDE_EAST:
            return (c->size.x / 2) * globalTransform.scale.x;
        case COLLIDER_SIDE_WEST:
            return (c->size.x / -2) * globalTransform.scale.x;
        case COLLIDER_SIDE_TOP:
            return (c->size.y / 2) * globalTransform.scale.y;
        case COLLIDER_SIDE_BOTTOM:
            return (c->size.y / -2) * globalTransform.scale.y;
    }
    return 0;
}

// do these collide
int collider_collides(scene *s, entity e1, entity e2) {

    collider *c1 = get_collider(s, e1);
    collider *c2 = get_collider(s, e2);
    if (c1 == NULL || c2 == NULL) return 0;

    transform g1 = get_global_transform(s, e1);
    transform g2 = get_global_transform(s, e2);

    vec3 scaledArea = c1->size;
    vec3 a = SUB(g1.pos, MUL_COMPONENTS(MUL(scaledArea, 0.5), g1.scale)); // a minimum
    vec3 A = ADD(a, scaledArea); // a maximum

    vec3 scaledArea2 = c2->size;
    vec3 b = SUB(g2.pos, MUL_COMPONENTS(MUL(scaledArea2, 0.5), g2.scale)); // b minimum
    vec3 B = ADD(b, scaledArea2); // b maximum

    if ((a.x < B.x && A.x > b.x) &&
        (a.y < B.y && A.y > b.y) &&
        (a.z < B.z && A.z > b.z)) {
        return 1;
    }

    return 0;
}

// destroy a region colldier (does nothing)
void collider_destroy(collider *_) {}