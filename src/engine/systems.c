
#include "microgame/engine/components/collider.h"
#include "microgame/engine/systems.h"
#include "microgame/engine/entity.h"
#include "microgame/engine/scene.h"
#include "microgame/util/math.h"
#include <stdio.h>

// TODO: use a smarter algorithmm (one that isn't O(n²))
// handle collision system
void collision_system_update(scene *s, entity first, float _) {

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

// get the sign of a number
static inline int sign(int a) {
    return a < 0 ? -1 : (a == 0 ? 0 : 1);
}

// get the first hit object
static entity velocity_system_first_collided(scene *s, entity e, vec3 pos, collider_side side) {

    // skip without collider
    if (!has_collider(s, e)) NIL_ENTITY;

    // get the collider
    collider *c = get_collider(s, e);
    vec3 a = vec3_sub(pos, vec3_mul(c->size, 2)); // a minimum
    vec3 A = vec3_add(a, c->size); // a maximum

    // closest distance and entity
    entity closest = NIL_ENTITY;
    float amnt = 0;

    // loop through other entities
    for (entity e2 = 0; e2 < MAX_ENTITIES; e2++) {

        // skip without collider
        if (!s->alive[e2] || e2 == e || !has_collider(s, e2)) continue;

        // get the collider
        collider *c2 = get_collider(s, e2);
        vec3 pos2 = get_global_transform(s, e2).pos;

        // check collision
        vec3 b = vec3_sub(pos2, vec3_mul(c2->size, 2)); // b minimum
        vec3 B = vec3_add(b, c2->size); // b maximum

        // check the bounds
        if ((a.x <= B.x && A.x >= b.x) &&
            (a.y <= B.y && A.y >= b.y) &&
            (a.z <= B.z && A.z >= b.z)) {

            // run functions for collision
            printf("hit.\n");
            // float dist = collider_get_offset(c2, side);
            // just return the first hit entity now for testing
            // TODO: find first hit entity (furthest away of given side)
            return e2;
        }
    }

    return NIL_ENTITY;
}

// axis
enum _axis {
    X,
    Y,
    Z
};

// the side you hit with given movement
static inline collider_side velocity_system_dir(enum _axis a, int dir) {
    if (a == X) {
        if (dir < 0)
            return COLLIDER_SIDE_EAST;
        return COLLIDER_SIDE_WEST;
    } else if (a == Y) {
        if (dir < 0)
            return COLLIDER_SIDE_TOP;
        return COLLIDER_SIDE_BOTTOM;
    }
    if (dir < 0)
        return COLLIDER_SIDE_SOUTH;
    return COLLIDER_SIDE_NORTH;
}

// handle a single axis
static int velocity_system_move_axis(
    float dt, 
    float dist, 
    float *dest) {
    
    // if there is movement
    if (dist != 0) {
        (*dest) += dist * dt;
        return 1;
    }

    // no movement
    return 0;
}

// handle collisions
static void velocity_system_resolve_axis(scene *s, entity e, vec3 *pos, enum _axis axis, int sn) {

    entity firstCollided = velocity_system_first_collided(s, e, *pos, velocity_system_dir(axis, sn));

    if (firstCollided != NIL_ENTITY) {
        printf("collided.\n");
    }
}

// update the velocity
void velocity_system_update(scene *s, entity e, float dt) {

    // skip if it does not have a velocity
    if (!has_velocity(s, e)) return;
    velocity *v = get_velocity(s, e);
    transform global = get_global_transform(s, e);

    // vars
    collider_side dir;
    entity firstHit;

    // move on the axes
    if (velocity_system_move_axis(dt, v->velocity.x, &global.pos.x))
        velocity_system_resolve_axis(s, e, &global.pos, X, sign(v->velocity.x));
    if (velocity_system_move_axis(dt, v->velocity.y, &global.pos.y))
        velocity_system_resolve_axis(s, e, &global.pos, Y, sign(v->velocity.y));
    if (velocity_system_move_axis(dt, v->velocity.z, &global.pos.z))
        velocity_system_resolve_axis(s, e, &global.pos, Z, sign(v->velocity.z));
    global.pos.x += v->velocity.x * dt;

    // global transform after all movement
    // TODO: store inverse matrices
    mat4 newGlobal = mat4_model(global.pos, global.rot, global.scale);

    // if parent
    entity parent = get_parent(s, e);
    if (parent != NIL_ENTITY) {
        mat4 inv = mat4_fast_inverse(get_world_transform_mat4(s, parent));
        newGlobal = mat4_mul(inv, newGlobal);
    }
    
    // move
    transform local = transform_from_mat(newGlobal);
    s->transforms[e] = local;
}