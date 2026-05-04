
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

// get the first hit object
static entity velocity_system_first_collided(scene *s, entity e, vec3 pos) {

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
static collider_side velocity_system_dir(enum _axis a, int dir) {
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

// update the velocity
void velocity_system_update(scene *s, entity e, float dt) {

    // skip if it does not have a velocity
    if (!has_velocity(s, e)) return;
    velocity *v = get_velocity(s, e);
    transform global = get_global_transform(s, e);

    // vars
    collider_side dir;
    entity firstHit;

    // move on the x axis
    global.pos.x += v->velocity.x * dt;
    if (v->velocity.x != 0) {
        firstHit = velocity_system_first_collided(s, e, global.pos);
        if (firstHit != NIL_ENTITY) {
            dir = velocity_system_dir(X, v->velocity.x);
            printf("dir: %d\n", (int)dir);
        }
    }

    // move on the y axis
    global.pos.y += v->velocity.y * dt;
    if (v->velocity.y != 0) {
        firstHit = velocity_system_first_collided(s, e, global.pos);
        if (firstHit != NIL_ENTITY) {
            dir = velocity_system_dir(Y, v->velocity.y);
            printf("dir: %d\n", (int)dir);
        }
    }

    // move on the z axis
    global.pos.z += v->velocity.z * dt;
    if (v->velocity.z != 0) {
        firstHit = velocity_system_first_collided(s, e, global.pos);
        if (firstHit != NIL_ENTITY) {
            dir = velocity_system_dir(Z, v->velocity.z);
            printf("dir: %d\n", (int)dir);
        }
    }

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