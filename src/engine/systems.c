
#include "microgame/engine/systems.h"
#include "microgame/engine/entity.h"
#include "microgame/engine/scene.h"
#include "microgame/util/math.h"
#include <stdio.h>

// handle collision system
void collision_system_update(scene *s, int maxEntities, float dt) {

    // for all entities
    for (entity first = 0; first < maxEntities; first++) {

        // skip if no collider or not alive
        if (!s->alive[first] || !s->has_collider[first]) continue;

        // get the vec3 pos and bounds
        vec3 globalPos, globalBounds;

        // for the other entities
        for (entity second = first + 1; second < maxEntities; second++) {

            // skip if no collider or not alive
            if (!s->alive[second] || !s->has_collider[second]) continue;

            // two have colliders
            printf("possible collision\n");
        }
    }
}