
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

    // make the mesh
    mesh m = mesh_from_obj(rgb_rand(), "assets/test.obj");
    
    // make a bunch of entities
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 3; i++) {

            // make a test monkey
            entity test = scene_spawn(s);
            scene_attach_mesh(s, test, m);
            get_transform(s, test)->pos.y = (i - 2) * 3;
            get_transform(s, test)->pos.x = (j - 2) * 3;
        }
    }

    // get the transform
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

        game_update(g);
    }

    // end
    game_destroy(g);
}