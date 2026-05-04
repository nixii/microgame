
#include "microgame/microgame.h"
#include <stdio.h>

// width height
#define WIDTH 1600
#define HEIGHT 1000
#define FPS 60
#define TITLE "Movement Test"

#define MOVE_SPEED 5.0
#define CAM_SPEED 1.7

// the game
microgame *game;

// scene info
scene *mainScene;

// camera information
entity cameraEntity;
transform *cameraTransform;
velocity *cameraVelocity;

// basic mesh stuff
mesh_resource testMesh;
mesh meshComponent;

// load the player
void loadPlayer() {
    cameraEntity = scene_spawn(mainScene);
    cameraTransform = get_transform(mainScene, cameraEntity);
    attach_collider(mainScene, cameraEntity, collider_new(vec3_new(1, 1, 1)));
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

    vec3 movement = vec3_zero();
    if (key_down(M_KEY_A))
        movement.x -= MOVE_SPEED;
    if (key_down(M_KEY_D))
        movement.x += MOVE_SPEED;
    if (key_down(M_KEY_W))
        movement.z += MOVE_SPEED;
    if (key_down(M_KEY_S))
        movement.z -= MOVE_SPEED;
    if (key_down(M_KEY_SPACE))
        movement.y += MOVE_SPEED;
    if (key_down(M_KEY_LCTRL))
        movement.y -= MOVE_SPEED;

    float rotYaw = (float)(key_down(M_KEY_RIGHT) - key_down(M_KEY_LEFT)) * CAM_SPEED * dt;
    float rotPitch = (float)(key_down(M_KEY_DOWN) - key_down(M_KEY_UP)) * CAM_SPEED * dt;
    printf("%f\n", rotYaw); // TODO: fix rotation
                            // TODO: make collision not snap to wrong side
    
    cameraTransform->rot.y += rotYaw;
    cameraTransform->rot.x += rotPitch;
    cameraVelocity->velocity = vec3_rotY(movement, cameraTransform->rot.y);
}

// run the game
int main(void) {

    // create the game
    game = game_new(WIDTH, HEIGHT, FPS, TITLE);

    // create the scene
    mainScene = scene_new();

    // load resources
    loadMeshes();

    // laod the player
    loadPlayer();

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