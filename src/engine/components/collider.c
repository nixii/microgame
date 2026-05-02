
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

// destroy a region colldier
void collider_destroy(collider *_) {}