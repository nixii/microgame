
#include "movement.h"
#include <stdio.h>

static vec3 vel = (vec3){0, 0, 0};
static vec3 impulses = (vec3){0, 0, 0};
float yVel = 0;

// dash cooldown
float dashcooldown = 0.0;
float canJump = 0;

// update the movement
void update_movement(scene *s, entity p) {
    float dt = get_dt();
    dashcooldown -= dt;

    // get the objects
    transform *cameraTransform = get_transform(s, p);
    collider *camCollider = get_collider(s, p);
    velocity *camVel = get_velocity(s, p);

    // rotate
    vec2 md = get_mouse_delta();
    float rotYaw = md.x * CAM_SPEED * dt;
    float rotPitch = md.y * CAM_SPEED * dt;
    cameraTransform->rot.y += rotYaw;
    cameraTransform->rot.x += rotPitch;
    cameraTransform->rot.x = cameraTransform->rot.x > PI / 2.2 ? PI / 2.2 : cameraTransform->rot.x;
    cameraTransform->rot.x = cameraTransform->rot.x < -PI / 2.2 ? -PI / 2.2 : cameraTransform->rot.x;

    // update the camera location
    s->camera.transform = *cameraTransform;

    // slow down
    if (camCollider->hitFloor) {
        vel = DIV(vel, GROUND_FRICTION);
        yVel = 0;
        canJump = 1;
    } else {
        vel = DIV(vel, AIR_FRICTION);
        yVel -= GRAVITY * dt;
    }

    // movement
    vec3 movement = vec3_zero();
    if (key_down(M_KEY_A))
        movement.x -= 1;
    if (key_down(M_KEY_D))
        movement.x += 1;
    if (key_down(M_KEY_W))
        movement.z += 1;
    if (key_down(M_KEY_S))
        movement.z -= 1;

    // dashing
    if (mouse_just_down(1) && dashcooldown <= 0) {
        impulses = vec3_rot_y(vec3_new(0, 0, DASH_SPEED), cameraTransform->rot.y);
        dashcooldown = DASH_COOLDOWN;
        yVel = 0;
    }

    // decay dash
    impulses = DIV(impulses, DASH_FRICTION);

    // apply the movement to the local vel
    movement = MUL(NORMALIZE(movement), MOVE_SPEED);
    vel.x += movement.x;
    vel.z += movement.z;

    // jumping
    if (key_just_down(M_KEY_SPACE) && canJump) {
        yVel = JUMP_POWER;
        canJump = 0;
    }

    // transform & apply the velocity
    camVel->velocity = ADD(vec3_rot_y(vec3_new(vel.x, yVel, vel.z), cameraTransform->rot.y), impulses);
}
