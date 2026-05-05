
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
        if ((a.x < B.x && A.x > b.x) &&
            (a.y < B.y && A.y > b.y) &&
            (a.z < B.z && A.z > b.z)) {

            // run functions for collision
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
static void velocity_system_resolve_axis(scene *s, entity e, vec3 *outpos, enum _axis axis, int sn) {

    if (sn == 0) return;

    // get the first entity you woulda hit
    collider_side side = velocity_system_dir(axis, sn);
    entity firstCollided = velocity_system_first_collided(s, e, *outpos, side);

    // snap to that entity if it exists
    if (firstCollided != NIL_ENTITY) {
        
        // get the side collided on
        collider *col = get_collider(s, e);
        collider *col2 = get_collider(s, firstCollided);

        // get positions
        vec3 pos = *outpos;
        vec3 pos2 = get_global_transform(s, firstCollided).pos;

        // get the half axes
        vec3 half = vec3_mul(col->size, 0.5);
        vec3 half2 = vec3_mul(col2->size, 0.5);

        // check cases
        if (axis == X) {
            if (sn > 0) {
                outpos->x = pos2.x - half2.x - half.x;
            } else if (sn < 0) {
                outpos->x = pos2.x + half2.x + half.x;
            }
        }

        // check the y axis
        else if (axis == Y) {
            if (sn > 0) {
                outpos->y = pos2.y - half2.y - half.y;
            } else if (sn < 0) {
                outpos->y = pos2.y + half2.y + half.y;
            }
        }

        // check the z axis
        else {
            if (sn > 0) {
                outpos->z = pos2.z - half2.z - half.z;
            } else if (sn < 0) {
                outpos->z = pos2.z + half2.z + half.z;
            }
        }
    }
}

// update the velocity
void velocity_system_update(scene *s, entity e, float dt) {

    // skip if it does not have a velocity
    if (!has_velocity(s, e)) return;
    velocity *v = get_velocity(s, e);
    transform global = get_global_transform(s, e);
    printf("rot 0: %f %f %f\n", global.rot.x, global.rot.y, global.rot.z);

    // vars
    collider_side dir;
    entity firstHit;

    // move on the axes
    int collided = 0;
    if (velocity_system_move_axis(dt, v->velocity.x, &global.pos.x)) {
        velocity_system_resolve_axis(s, e, &global.pos, X, sign(v->velocity.x));
        collided = 1;
    }
    if (velocity_system_move_axis(dt, v->velocity.y, &global.pos.y)) {
        velocity_system_resolve_axis(s, e, &global.pos, Y, sign(v->velocity.y));
        collided = 1;
    }
    if (velocity_system_move_axis(dt, v->velocity.z, &global.pos.z)) {
        velocity_system_resolve_axis(s, e, &global.pos, Z, sign(v->velocity.z));
        collided = 1;
    }

    printf("rot 1: %f %f %f\n", global.rot.x, global.rot.y, global.rot.z);

    // if any collision
    if (collided) {

        // global transform after all movement
        // TODO: cache inverse matrices
        mat4 newGlobal = mat4_model(global.pos, global.rot, global.scale);

        // if parent
        entity parent = get_parent(s, e);
        if (parent != NIL_ENTITY) {
            mat4 inv = mat4_fast_inverse(get_world_transform_mat4(s, parent));
            newGlobal = mat4_mul(inv, newGlobal);
        }
        
        // move
        transform local = transform_from_mat(newGlobal);
        mat4_display(newGlobal);
        s->transforms[e] = local;
    }
}