
#include "microengine/scene.h"
#include "microengine/components.h"
#include "microgame/microgame.h"
#include <stdio.h>
#include <math.h>

int main(void)
{
    scene *s = scene_new();
    s->cam.transform.pos = vec3_new(0, 3, -10);
    s->cam.transform.rot.x = 0.4;

    entity e = scene_spawn(s);
    me_scene_attach_transform(s, e, me_transform_blank());

    // a cube
    vec3 v0 = vec3_new(-0.5, -0.5, -0.5);
    vec3 v1 = vec3_new( 0.5, -0.5, -0.5);
    vec3 v2 = vec3_new( 0.5,  0.5, -0.5);
    vec3 v3 = vec3_new(-0.5,  0.5, -0.5);
    vec3 v5 = vec3_new( 0.5, -0.5, 0.5);
    vec3 v4 = vec3_new(-0.5, -0.5, 0.5);
    vec3 v6 = vec3_new( 0.5,  0.5, 0.5);
    vec3 v7 = vec3_new(-0.5,  0.5, 0.5);

    // attach the cube
    me_scene_attach_mesh(s, e, mesh_from(36,
        v0, v1, v2,
        v0, v2, v3,
        v5, v4, v7,
        v5, v7, v6,
        v4, v0, v3,
        v4, v3, v7,
        v1, v5, v6,
        v1, v6, v2,
        v4, v5, v1,
        v4, v1, v0,
        v3, v2, v6,
        v3, v6, v7
    ));

    // init the game
    game *g = microgame_init(600, 600, "test", 60);
    microgame_set_scene(g, s);

    transform *tr = me_scene_get_transform(s, e);

    int frame = 0;

    while (microgame_running(g))
    {
        frame++;

        // move square
        tr->pos.z = 5.0 - (frame / 10.0);
        tr->rot.x += 0.05;

        microgame_render(g);
    }

    microgame_end(g);
    return 0;
}