
#include "microengine/scene.h"
#include "microengine/components.h"
#include "microgame/microgame.h"
#include <stdio.h>
#include <math.h>

int main(void)
{
    scene *s = scene_new();
    entity e = scene_spawn(s);
    me_scene_attach_transform(s, e, transform_new(
        vec3_new(40, 40, 10),
        vec3_new(0, 0, 0),
        vec3_new(1, 1, 1)
    ));
    me_scene_attach_mesh(s, e, mesh_from(3,
        vec3_new(10, 10, 30),
        vec3_new(300, 180, 30),
        vec3_new(40, 210, 30)
    ));

    game *g = microgame_init(600, 400, "test", 60);
    microgame_set_scene(g, s);

    int frame = 0;

    while (microgame_running(g))
    {
        frame++;

        // move square
        transform *t = me_scene_get_transform(s, e);
        t->pos.y = (sin((double)frame / 30) * 150) + 150;
        t->pos.x = frame;

        microgame_render(g);
    }

    microgame_end(g);
    return 0;
}