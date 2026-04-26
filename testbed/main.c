
#include "microgame/microgame.h"

int main(void)
{
    microgame *g = game_new(600, 400, 30, "hi world");
    
    scene *s = scene_new();
    entity e = scene_spawn(s);
    transform *t = get_transform(s, e);
    t->pos.x = 100;
    t->pos.y= 30;
    t->pos.z = 50;

    game_set_scene(g, s);
    while (game_still_running(g)) {
        game_update(g);
    }

    game_destroy(g);
}