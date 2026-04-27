
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
    microgame *g = game_new(600, 400, 60, "hi world");
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

    // make a test triangle
    entity test = scene_spawn(s);
    mesh *m = scene_attach_mesh(s, test, mesh_new(rgb_rand(), 36, (vec3[]){
        
        // Back face (z = 6)
        cube[1], cube[5], cube[3],
        cube[3], cube[5], cube[7],

        // Front face (z = 5)
        cube[0], cube[2], cube[4],
        cube[2], cube[6], cube[4],

        // Left face (x = -0.5)
        cube[0], cube[4], cube[1],
        cube[1], cube[4], cube[5],

        // Right face (x = 0.5)
        cube[2], cube[3], cube[6],
        cube[3], cube[7], cube[6],

        // Bottom face (y = -0.5)
        cube[0], cube[1], cube[2],
        cube[2], cube[1], cube[3],

        // Top face (y = 0.5)
        cube[4], cube[6], cube[5],
        cube[5], cube[6], cube[7],
    }));

    // get the transform
    transform *t = get_transform(s, test);
    camera *c = &s->camera;
    c->transform.pos = vec3_new(0, 3, -10);

    // set the scene
    game_set_scene(g, s);
    int frame = 0;

    // update
    while (game_still_running(g)) {

        // rotate the camera
        float rotYaw = (float)(key_down(M_KEY_RIGHT) - key_down(M_KEY_LEFT)) / 60.0;
        c->transform.rot.y += rotYaw;
        float rotPitch = (float)(key_down(M_KEY_DOWN) - key_down(M_KEY_UP)) / 60.0;
        c->transform.rot.x += rotPitch;

        // move the camera
        if (key_down(M_KEY_A))
            c->transform.pos.x -= 1.0 / 30;
        if (key_down(M_KEY_D))
            c->transform.pos.x += 1.0 / 30;
        if (key_down(M_KEY_W))
            c->transform.pos.z += 1.0 / 30;
        if (key_down(M_KEY_S))
            c->transform.pos.z -= 1.0 / 30;
        if (key_down(M_KEY_SPACE))
            c->transform.pos.y += 1.0 / 30;
        if (key_down(M_KEY_LSHIFT))
            c->transform.pos.y -= 1.0 / 30;

        game_update(g);
        frame++;
    }

    // end
    game_destroy(g);
}