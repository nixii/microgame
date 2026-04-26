
#include "microgame/microgame.h"
#include "microgame/engine/components/mesh.h"
#include "microgame/util/math.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

int main(void)
{

    // create the game
    microgame *g = game_new(600, 400, 30, "hi world");
    scene *s = scene_new();

    // cube points
    vec3 cube[8] = {
        vec3_new(-0.5, -0.5, 5),
        vec3_new(-0.5, -0.5, 6),
        vec3_new(0.5, -0.5, 5),
        vec3_new(0.5, -0.5, 6),
        vec3_new(-0.5, 0.5, 5),
        vec3_new(-0.5, 0.5, 6),
        vec3_new(0.5, 0.5, 5),
        vec3_new(0.5, 0.5, 6),
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

    // set the scene
    game_set_scene(g, s);
    int frame = 0;

    // update
    while (game_still_running(g)) {

        t->pos.z = 5 + sinf((float)frame / 30) * 9;

        game_update(g);
        frame++;
    }

    // end
    game_destroy(g);
}