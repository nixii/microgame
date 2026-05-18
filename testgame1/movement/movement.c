
#include <microgame/engine/resource/sound.h>
#include "../scenes/scene_manager.h"
#include "../scenes/util.h"
#include "movement.h"
#include <stdio.h>

static vec3 vel = (vec3){0, 0, 0};
static vec3 impulses = (vec3){0, 0, 0};
float yVel = 0;
static int numRescues = 0;
static char rescuesBuf[64];

static float jumpCheck = 0.0;

static sound jumpSound;
static sound dashSound;
static sound fallSound;

// dash cooldown
float dashcooldown = 0.0;
float canJump = 0;

// init movement
void initMovement() {
    jumpSound = sound_from("testgame1/assets/jump.wav");
    dashSound = sound_from("testgame1/assets/dash.wav");
    fallSound = sound_from("testgame1/assets/fall.wav");
}

// update the movement
void update_movement(scene *s, entity p) {
    if (p == NIL_ENTITY) return;
    float dt = get_dt();
    dashcooldown -= dt;
    jumpCheck += dt;

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
        canJump = 2;
    } else {
        vel = DIV(vel, AIR_FRICTION);
        yVel -= GRAVITY * dt;
        // canJump = canJump > 0 ? 1 : 0;
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
    if (key_down(M_KEY_LSHIFT) && camCollider->size.y != 0.5) {
        camCollider->size.y = 0.5;
        cameraTransform->pos.y -= 0.25;
    }
    else if (!key_down(M_KEY_LSHIFT) && camCollider->size.y == 0.5) {
        camCollider->size.y = 1;
        cameraTransform->pos.y += 0.25;
    }

    // dashing
    if (mouse_just_down(1) && dashcooldown <= 0) {
        impulses = vec3_rot_y(vec3_new(0, 0, DASH_SPEED), cameraTransform->rot.y);
        dashcooldown = DASH_COOLDOWN;
        yVel = 0;
        sound_play(&dashSound);
    }

    // decay dash
    impulses = DIV(impulses, DASH_FRICTION);

    // apply the movement to the local vel
    movement = MUL(NORMALIZE(movement), MOVE_SPEED);
    vel.x += movement.x;
    vel.z += movement.z;

    // jumping
    if (key_just_down(M_KEY_SPACE)) {
        jumpCheck = -0.25;
    }
    if (jumpCheck <= 0.0 && canJump) {
        yVel = JUMP_POWER;
        canJump -= 1;
        jumpCheck = 0.0;
        sound_play(&jumpSound);
    }

    // transform & apply the velocity
    camVel->velocity = ADD(vec3_rot_y(vec3_new(vel.x, yVel, vel.z), cameraTransform->rot.y), impulses);
}

void rescue(scene *s, entity p) {
    if (p == NIL_ENTITY) return;
    if (get_transform(s, p)->pos.y < -5) {
        get_transform(s, p)->pos = get_spawn_pos();
        get_velocity(s, p)->velocity = vec3_zero();
        numRescues += 1;
        snprintf(rescuesBuf, sizeof(rescuesBuf), "You have been rescued %d times.", numRescues);
        ui_text_set_text(get_dialogue(), rescuesBuf);
        sound_play(&fallSound);
    }
}