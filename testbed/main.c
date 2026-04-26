
#include "microgame/microgame.h"
#include "microgame/engine/components/mesh.h"
#include "microgame/util/math.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

int main(void)
{
    
    // create the game
    microgame *g = game_new(600, 400, 60, "hi world");
    scene *s = scene_new();

    // set the scene
    game_set_scene(g, s);
    int frame = 0;

    // update
    while (game_still_running(g)) {
        game_update(g);
        frame++;
    }

    // end
    game_destroy(g);
}