
#ifndef MG_COMPONENTS_VELOCITY_H
#define MG_COMPONENTS_VELOCITY_H

#include "microgame/util/math.h"

// create velocity component
typedef struct velocity {
    vec3 velocity;
} velocity;

// create a new velocity
velocity velocity_new(vec3 initial);

// delete a velocity
void velocity_destroy(velocity *v);

#endif // MG_COMPONENTS_VELOCITY_H