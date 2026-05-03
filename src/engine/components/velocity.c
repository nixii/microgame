
#include "microgame/engine/components/velocity.h"

// create a new velocity
velocity velocity_new(vec3 initial) {
    return (velocity){
        .velocity = initial
    };
}

// delete a velocity
void velocity_destroy(velocity *_) {}