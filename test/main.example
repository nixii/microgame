
#include <microgame/microgame.h>
#include <raylib.h>
#include <stdio.h>

#define WIDTH 600
#define HEIGHT 400

int main(void)
{

    // make the main scene
    mg_scene main = mg_scene_new();

    // create an entity
    int myEntity = mg_scene_spawn(&main);

    // make a mesh
    mg_mesh myMesh = mg_mesh(3, 10, 10, 10);

    // attach the mesh
    mg_scene_attach_mesh(&main, myEntity, myMesh);

    mg_game g = mg_init(WIDTH, HEIGHT);
    

    // all done!
    return 0;
}