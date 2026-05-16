
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
        if ((a.x < B.x && A.x > b.x) &&
            (a.y < B.y && A.y > b.y) &&
            (a.z < B.z && A.z > b.z)) {

            // run functions for collision
            if (firstCol->onCollision != NULL)
                firstCol->onCollision(first, second);
            if (secondCol->onCollision != NULL)
                secondCol->onCollision(second, first);
        }
    }
}

// get the sign of a number
static inline int sign(float a) {
    return a < 0 ? -1 : (a == 0 ? 0 : 1);
}

// TODO: make this get only the FIRST hit entity.
// get the first hit object
static entity velocity_system_first_collided(scene *s, entity e, transform *transf, collider_side side) {

    // skip without collider
    if (!has_collider(s, e)) return NIL_ENTITY;

    // get the collider
    collider *c = get_collider(s, e);
    vec3 scaledArea = c->size;
    scaledArea = MUL_COMPONENTS(scaledArea, transf->scale);
    vec3 a = SUB(transf->pos, MUL(scaledArea, 0.5)); // a minimum
    vec3 A = ADD(a, scaledArea); // a maximum

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
        vec3 scale2 = get_global_transform(s, e2).scale;

        // check collision
        vec3 scaledArea2 = c2->size;
        scaledArea2 = MUL_COMPONENTS(scaledArea2, scale2);
        vec3 b = SUB(pos2, MUL(scaledArea2, 0.5)); // b minimum
        vec3 B = ADD(b, scaledArea2); // b maximum

        // check the bounds
        if ((a.x < B.x && A.x > b.x) &&
            (a.y < B.y && A.y > b.y) &&
            (a.z < B.z && A.z > b.z)) {

            // run functions for collision
            // float dist = collider_get_offset(c2, side);
            // just return the first hit entity now for testing
            // TODO: find first hit entity (furthest away of given side)
            switch (side) {
                case COLLIDER_SIDE_BOTTOM:
                    c->hitCeiling = 1;
                    c2->hitCeiling = 1;
                    break;
                case COLLIDER_SIDE_TOP:
                    c->hitFloor = 1;
                    c2->hitFloor = 1;
                    break;
                default:
                    c->hitWall = 1;
                    c2->hitWall = 1;
            }
            return e2;
        } else {
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
static void velocity_system_resolve_axis(scene *s, entity e, transform *outpos, enum _axis axis, int sn) {

    if (sn == 0) return;

    // get the first entity you woulda hit
    collider_side side = velocity_system_dir(axis, sn);
    entity firstCollided = velocity_system_first_collided(s, e, outpos, side);

    // snap to that entity if it exists
    if (firstCollided != NIL_ENTITY) {
        
        // get the side collided on
        collider *col = get_collider(s, e);
        collider *col2 = get_collider(s, firstCollided);

        // get positions
        vec3 pos = outpos->pos;
        transform transf2 = get_global_transform(s, firstCollided);
        vec3 pos2 = transf2.pos;

        // get the half axes
        vec3 half = MUL_COMPONENTS(MUL(col->size, 0.5), outpos->scale);
        vec3 half2 = MUL_COMPONENTS(MUL(col2->size, 0.5), transf2.scale);

        // check cases
        if (axis == X) {
            if (sn > 0) {
                outpos->pos.x = pos2.x - half2.x - half.x - COLLISION_EPSILON;
            } else if (sn < 0) {
                outpos->pos.x = pos2.x + half2.x + half.x + COLLISION_EPSILON;
            }
        }

        // check the y axis
        else if (axis == Y) {
            if (sn > 0) {
                outpos->pos.y = pos2.y - half2.y - half.y - COLLISION_EPSILON;
            } else if (sn < 0) {
                outpos->pos.y = pos2.y + half2.y + half.y + COLLISION_EPSILON;
            }
        }

        // check the z axis
        else {
            if (sn > 0) {
                outpos->pos.z = pos2.z - half2.z - half.z - COLLISION_EPSILON;
            } else if (sn < 0) {
                outpos->pos.z = pos2.z + half2.z + half.z + COLLISION_EPSILON;
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

    // move  on axes
    int mX = velocity_system_move_axis(dt, v->velocity.x, &global.pos.x);
    if (mX) velocity_system_resolve_axis(s, e, &global, X, sign(v->velocity.x));
    int mY = velocity_system_move_axis(dt, v->velocity.y, &global.pos.y);
    if (mY) velocity_system_resolve_axis(s, e, &global, Y, sign(v->velocity.y));
    int mZ = velocity_system_move_axis(dt, v->velocity.z, &global.pos.z);
    if (mZ) velocity_system_resolve_axis(s, e, &global, Z, sign(v->velocity.z));

    // if any collision
    if (mX || mY || mZ) {

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
        s->transforms[e] = local;
    }
}