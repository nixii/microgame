
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
    if (!s->alive[first] || !has_collider(s, first)) return;

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

// get the first hit object
static void velocity_system_first_collided(scene *s, entity e) {

    // skip without collider
    if (!has_collider(s, e)) return;

    // get the collider
    collider *c = get_collider(s, e);

    // loop through other entities
    for (entity e2 = 0; e2 < MAX_ENTITIES; e2++) {

        // skip without collider
        if (!s->alive[e2] || e2 == e || !has_collider(s, e2)) continue;

        // get the collider
        collder *c2 = get_collider(s, e2);
    }
}

// update the velocity
void velocity_system_update(scene *s, entity e, float _) {

    // skip if it does not have a velocity
    if (!has_velocity(s, e)) return;
    velocity *v = get_velocity(s, e);
    transform global = get_global_transform(s, e);

    // move on the x axis
    global.pos.x += v->velocity.x;
    entity firstHit = velocity_system_first_collided(s, e);

    // move on the y axis
    global.pos.y += v->velocity.y;
    firstHit = velocity_system_first_collided(s, e);

    // move on the z axis
    global.pos.z += v->velocity.z;
    firstHit = velocity_system_first_collided(s, e);

    // global transform after everything
}