
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

    // make a test triangle
    entity test = scene_spawn(s);
    mesh *m = scene_attach_mesh(s, test, mesh_new(rgb_rand(), 3, (vec3[]){
        vec3_new(0.5, 0.5, 60),
        vec3_new(-0.5, 0.5, 60),
        vec3_new(0.5, -0.5, 60)
    }));

    // set the scene
    game_set_scene(g, s);
    int frame = 0;

    // update
    while (game_still_running(g)) {

        for (int i = 0; i < m->numVerts; i++) {
            m->verts[i].z -= (float)frame / 120;
        }

        game_update(g);
        frame++;
    }

    // end
    game_destroy(g);
}