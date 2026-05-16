
#include "movement.h"

static float negYVel = 0;   
void update_movement(scene *s, entity p) {
    float dt = get_dt();
    transform *cameraTransform = get_transform(s, p);
    collider *camCollider = get_collider(s, p);
    velocity *cameraVelocity = get_velocity(s, p);

    s->camera.transform = *cameraTransform;

    // movement
    vec3 movement = vec3_zero();
    if (key_down(M_KEY_A))
        movement.x -= MOVE_SPEED;
    if (key_down(M_KEY_D))
        movement.x += MOVE_SPEED;
    if (key_down(M_KEY_W))
        movement.z += MOVE_SPEED;
    if (key_down(M_KEY_S))
        movement.z -= MOVE_SPEED;
    if (key_just_down(M_KEY_SPACE))
        negYVel = 4;

    // look around
    vec2 mD = get_mouse_delta();

    // move based on delta
    float rotYaw = mD.x * CAM_SPEED * dt;
    float rotPitch = mD.y * CAM_SPEED * dt;
    
    // rotation!
    cameraTransform->rot.y += rotYaw;
    cameraTransform->rot.x += rotPitch;
    cameraTransform->rot.x = cameraTransform->rot.x > PI / 3 ? PI / 3 : cameraTransform->rot.x;
    cameraTransform->rot.x = cameraTransform->rot.x < -PI / 3 ? -PI / 3 : cameraTransform->rot.x;
    vec3 mov = vec3_rot_y(movement, cameraTransform->rot.y);
    cameraVelocity->velocity.x = mov.x;
    cameraVelocity->velocity.z = mov.z;

    // -y vel
    negYVel -= GRAVITY * dt;

    // did it collide
    if (camCollider->hitFloor || camCollider->hitCeiling) {
        negYVel = maxf(0, negYVel);
    }
    
    // set the y part
    cameraVelocity->velocity.y = negYVel;
}