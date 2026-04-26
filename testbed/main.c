
#include "microgame/microgame.h"
#include "microgame/engine/components/mesh.h"
#include "microgame/util/math.h"
#include <math.h>

int main(void)
{
    microgame *g = game_new(600, 400, 30, "hi world");
    
    scene *s = scene_new();
    entity e = scene_spawn(s);
    transform *t = get_transform(s, e);
    t->pos.x = 100;
    t->pos.y= 30;
    t->pos.z = 50;

    mesh *m = scene_attach_mesh(s, e, mesh_new(3,
        vec3_new(10, 10, 10),
        vec3_new(100, 20, 10),
        vec3_new(150, 300, 10)
    ));

    vec3 *p3 = &m->verts[2];

    game_set_scene(g, s);
    int frame = 0;
    while (game_still_running(g)) {

        p3->x = 300 + sin((float)frame / 30) * 250;

        game_update(g);
        frame++;
    }

    game_destroy(g);
}