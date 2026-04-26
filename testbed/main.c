
#include "microengine/scene.h"
#include "microengine/components.h"
#include "microgame/microgame.h"
#include <stdio.h>
#include <math.h>

int main(void)
{
    scene *s = scene_new();
    entity e = scene_spawn(s);
    me_scene_attach_transform(s, e, me_transform_blank());
    me_scene_attach_mesh(s, e, mesh_from(3,
        vec3_new(-0.5, 0.5, 5),
        vec3_new(0.5, 0.5, 5),
        vec3_new(-0.5, -0.5, 5)
    ));
    s->cam.transform.pos.z = -10;

    game *g = microgame_init(600, 400, "test", 60);
    microgame_set_scene(g, s);

    transform *tr = me_scene_get_transform(s, e);

    int frame = 0;

    while (microgame_running(g))
    {
        frame++;

        // move square
        tr->pos.z = 5.0 - (frame / 10.0);

        microgame_render(g);
    }

    microgame_end(g);
    return 0;
}