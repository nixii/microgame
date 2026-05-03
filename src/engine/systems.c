
#include "microgame/engine/components/collider.h"
#include "microgame/engine/systems.h"
#include "microgame/engine/entity.h"
#include "microgame/engine/scene.h"
#include "microgame/util/math.h"
#include <stdio.h>

// TODO: use a smarter algorithmm (one that isn't O(n²))
// handle collision system
void collision_system_update(scene *s, entity first, float dt) {

    // skip if no collider or not alive
    if (!s->alive[first] || !has_collider(s, first)) continue;

    // get the vec3 pos and bounds
    collider *firstCol = get_collider(s, first);
    transform firstTrans = get_global_transform(s, first);
    vec3 a = vec3_sub(firstTrans.pos, vec3_mul(firstCol->size, 2)); // a minimum
    vec3 A = vec3_add(a, firstCol->size); // a maximum

    // for the other entities
    for (entity second = first + 1; second < MAX_ENTITIES; second++) {

        // skip if no collider or not alive
        if (!s->alive[second] || !has_collider(s, second)) continue;

        // get the bounds
        collider *secondCol = get_collider(s, second);
        transform secondTrans = get_global_transform(s, second);
        vec3 b = vec3_sub(secondTrans.pos, vec3_mul(secondCol->size, 2)); // b minimum
        vec3 B = vec3_add(b, secondCol->size); // b maximum

        // check the bounds
        if ((a.x <= B.x && A.x >= b.x) &&
            (a.y <= B.y && A.y >= b.y) &&
            (a.z <= B.z && A.z >= b.z)) {

            // run functions for collision
            if (firstCol->onCollision != NULL)
                firstCol->onCollision(first, second);
            if (secondCol->onCollision != NULL)
                secondCol->onCollision(second, first);
        }
    }
}

// update the velocity
void velocity_system_update(scene *s, entity e, float dt) {
    printf("veloicty update ooo!\n");
}