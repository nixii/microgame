
#include "microgame/microgame.h"
#include "microgame/engine/components/mesh.h"
#include "microgame/util/math.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

int main(void)
{
    // actual random seed
    srand(time(NULL));

    // create the game
    microgame *g = game_new(600, 400, 60, "hi world");
    scene *s = scene_new();

    // meshes
    int numMeshes = 100;
    mesh *meshes[numMeshes];

    for (int i = 0; i < numMeshes; i++) {
        entity e = scene_spawn(s);
        meshes[i] = scene_attach_mesh(s, e, mesh_new(rgb_rand(), 3, (vec3[]){
            vec3_new(rand() % 600, rand() % 400, 0),
            vec3_new(rand() % 600, rand() % 400, 0),
            vec3_new(rand() % 600, rand() % 400, 0)
        }));
    }

    game_set_scene(g, s);
    int frame = 0;

    while (game_still_running(g)) {

        for (int i = 0; i < numMeshes; i++) {
            meshes[i]->verts[0] = vec3_new(rand() % 600, rand() % 400, 0);
            meshes[i]->verts[1] = vec3_new(rand() % 600, rand() % 400, 0);
            meshes[i]->verts[2] = vec3_new(rand() % 600, rand() % 400, 0);
        }

        game_update(g);
        frame++;
    }

    game_destroy(g);
}