
#include "microgame/microgame.h"
#include "microgame/engine/components/mesh.h"
#include "microgame/util/math.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

int main(void)
{

    // random seed
    srand(time(NULL));

    // create the game
    microgame *g = game_new(1600, 1000, 60, "hi world");
    scene *s = scene_new();

    // cube points
    vec3 cube[8] = {
        vec3_new(-0.5, -0.5, -0.5),
        vec3_new(-0.5, -0.5, 0.5),
        vec3_new(0.5, -0.5, -0.5),
        vec3_new(0.5, -0.5, 0.5),
        vec3_new(-0.5, 0.5, -0.5),
        vec3_new(-0.5, 0.5, 0.5),
        vec3_new(0.5, 0.5, -0.5),
        vec3_new(0.5, 0.5, 0.5),
    };

    // make a test monkey
    entity test = scene_spawn(s);
    // scene_attach_mesh(s, test, mesh_new(rgb_rand(), 8, cube, 36, (int []){
        
    //     // Front
    //     0, 4, 2,
    //     2, 4, 6,

    //     // Back
    //     1, 3, 5,
    //     3, 7, 5,

    //     // Left
    //     0, 1, 4,
    //     1, 5, 4,

    //     // Right
    //     2, 6, 3,
    //     3, 6, 7,

    //     // Bottom
    //     0, 2, 1,
    //     1, 2, 3,

    //     // Top
    //     4, 5, 6,
    //     5, 7, 6
    // }));
    scene_attach_mesh(s, test, mesh_from_obj(rgb_rand(), "assets/test.obj"));
    transform *eTransform = get_transform(s, test);

    // get the transform
    get_transform(s, test);
    camera *c = &s->camera;
    c->transform.pos = vec3_new(0, 3, -10);

    // set the scene
    game_set_scene(g, s);

    // speed
    float camSpeed = 1.7; // 1.7 rad a second
    float moveSpeed = 5.0; // 5 meters a second

    // how many frames
    int frameCount = 0;

    // update
    while (game_still_running(g)) {
        frameCount++;

        float dt = get_dt();

        // rotate the camera
        float rotYaw = (float)(key_down(M_KEY_RIGHT) - key_down(M_KEY_LEFT)) * camSpeed * dt;
        c->transform.rot.y += rotYaw;
        float rotPitch = (float)(key_down(M_KEY_DOWN) - key_down(M_KEY_UP)) * camSpeed * dt;
        c->transform.rot.x += rotPitch;

        // get the cam movement
        vec3 movement = vec3_zero();
        if (key_down(M_KEY_A))
            movement.x -= moveSpeed * dt;
        if (key_down(M_KEY_D))
            movement.x += moveSpeed * dt;
        if (key_down(M_KEY_W))
            movement.z += moveSpeed * dt;
        if (key_down(M_KEY_S))
            movement.z -= moveSpeed * dt;
        if (key_down(M_KEY_SPACE))
            movement.y += moveSpeed * dt;
        if (key_down(M_KEY_LCTRL))
            movement.y -= moveSpeed * dt;
        
        // rotate the vector
        movement = vec3_rotY(movement, c->transform.rot.y);
        c->transform.pos = vec3_add(c->transform.pos, movement);

        // move the thing
        eTransform->pos.y = sin((float)frameCount / 30) * 3;

        game_update(g);
    }

    // end
    game_destroy(g);
}