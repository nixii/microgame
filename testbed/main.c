
#include "microgame/microgame.h"
#include <stdio.h>

// width height
#define WIDTH 1000
#define HEIGHT 800
#define FPS 60
#define TITLE "Movement Test"

#define MOVE_SPEED 5.0
#define CAM_SPEED 0.25
#define GRAVITY 8

// the game
microgame *game;

// scene info
scene *mainScene;

// camera information
entity cameraEntity;
transform *cameraTransform;
velocity *cameraVelocity;
collider *camCollider;
float negYVel = 0;

// basic mesh stuff
mesh_resource testMesh;
mesh meshComponent;

// load the player
void loadPlayer() {
    cameraEntity = scene_spawn(mainScene);
    cameraTransform = get_transform(mainScene, cameraEntity);
    camCollider = attach_collider(mainScene, cameraEntity, collider_new(vec3_new(1, 3, 1)));
    cameraVelocity = attach_velocity(mainScene, cameraEntity, velocity_new(vec3_zero()));
}

// load meshes
void loadMeshes() {
    testMesh = mesh_resource_from_obj("assets/test.obj");
    meshComponent = mesh_from_resource(rgb_rand(), testMesh);
}

// spawn a test entity
void spawnTestEntity(vec3 pos) {
    entity e = scene_spawn(mainScene);
    attach_mesh(mainScene, e, meshComponent);
    get_transform(mainScene, e)->pos = pos;
    get_transform(mainScene, e)->scale = vec3_new(0.5, 0.5, 0.5);
    attach_collider(mainScene, e, collider_new(vec3_new(1, 1, 1)));
}

// allow movement
void handleMovement(float dt) {
    mainScene->camera.transform = *cameraTransform;

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
    if (camCollider->collided) {
        negYVel = maxf(0, negYVel);
    }
    
    // set the y part
    cameraVelocity->velocity.y = negYVel;
}

// floor
mesh_resource floorCube;
void spawnFloor() {
    entity e = scene_spawn(mainScene);

    floorCube = mesh_resource_from_obj("assets/cube.obj");
    attach_mesh(mainScene, e, mesh_from_resource(rgb(100, 0, 100), floorCube));
    attach_collider(mainScene, e, collider_new(vec3_new(30, 1, 30)));

    get_transform(mainScene, e)->scale = vec3_new(30, 1, 30);
    get_transform(mainScene, e)->pos.y = -2;
}

// run the game
int main(void) {

    // create the game
    game = game_new(WIDTH, HEIGHT, FPS, TITLE);
    disable_mouse();

    // create the scene
    mainScene = scene_new();

    // load resources
    loadMeshes();

    // laod the player
    loadPlayer();

    // floor
    spawnFloor();

    // load the static entity
    spawnTestEntity(vec3_new(2, 0, 0));

    // set the scene
    game_set_scene(game, mainScene);

    // run the game
    while (game_still_running(game)) {

        // get deltatime
        float dt = get_dt();

        // update movement
        handleMovement(dt);

        // update the game
        game_update(game);
    }
}