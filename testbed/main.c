
#include "microengine/scene.h"
#include "microengine/components.h"
#include "microgame/microgame.h"

int main(void)
{
    scene *s = scene_new();
    entity e = scene_spawn(s);
    scene_attach_transform(s, e, transform_clear());

    microgame *g = microgame_init(600, 400, "test", 30);
    microgame_set_scene(g, s);

    while (microgame_running(g))
    {
        microgame_render(g);
    }

    microgame_end(g);
    return 0;
}