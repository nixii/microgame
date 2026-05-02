
#include "microgame/engine/components/region_collider.h"

/*
 * region collider functions
*/

// create a new region collider
region_collider region_collider_new(vec3 size, int centered) {
    return (region_collider){
        .size = size,
        .centered = centered
    };
}

// destroy a region colldier
void region_collider_destroy(region_collider *c) {}