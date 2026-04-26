
#include <math.h>
#include <stdio.h>
#include "microengine/engine.h"

me_vec2 project_to_vec2(me_vec3 v, me_transform t, me_camera cam, int width, int height)
{

    // model to world
    v = me_vec3_rotate(v, t.rot);
    v.x += t.pos.x;
    v.y += t.pos.y;
    v.z += t.pos.z;

    // world to view
    v.x -= cam.transform.pos.x;
    v.y -= cam.transform.pos.y;
    v.z -= cam.transform.pos.z;

    v = me_vec3_rotate(v, (me_vec3){
        -cam.transform.rot.x,
        -cam.transform.rot.y,
        -cam.transform.rot.z
    });

    // projection
    if (v.z <= 0.4) return (me_vec2){-1, -1};

    float f = 1.0f / tanf(cam.fov * 0.5f);

    float px = (v.x * f) / v.z;
    float py = (v.y * f) / v.z;

    return (me_vec2){
        (px + 1.0f) * 0.5f * width,
        (1.0f - py) * 0.5f * height
    };
}

void microengine_render(me_scene *s, mr_renderer *r)
{

    // clear the screen
    mr_renderer_clear(r, mr_rgb(0, 0, 0));

    // camera
    me_camera cam = s->cam;

    // load all the triangles
    for (entity e = 0; e < ME_MAX_ENTITIES; e++)
    {
        if (!s->alive[e]) continue;

        // render triangle
        if (me_scene_has_mesh(s, e) && me_scene_has_transform(s, e))
        {
            me_mesh *m = me_scene_get_mesh(s, e);
            me_transform t = *me_scene_get_transform(s, e);

            // for each pair of 3 vertices
            for (int i = 0; i < m->vertCount; i += 3)
            {
                
                // get the vertices
                me_vec3 v1 = m->verts[i];
                me_vec3 v2 = m->verts[i + 1];
                me_vec3 v3 = m->verts[i + 2];
                
                // simple project it
                me_vec2 p1 = project_to_vec2(v1, t, cam, r->width, r->height);
                me_vec2 p2 = project_to_vec2(v2, t, cam, r->width, r->height);
                me_vec2 p3 = project_to_vec2(v3, t, cam, r->width, r->height);

                // skip if incorrect
                if (p1.x == p1.y && p1.x == -1) continue;
                if (p2.x == p2.y && p2.x == -1) continue;
                if (p3.x == p3.y && p3.x == -1) continue;

                printf("point: %f %f %f %f %f %f\n", 
                    p1.x, p1.y,
                    p2.x, p2.y,
                    p3.x, p3.y);
                
                // draw the triangle
                mr_renderer_render_triangle(r,
                    p1.x, p1.y,
                    p2.x, p2.y,
                    p3.x, p3.y,
                    mr_rgb(255, 0, 255)
                );
            }
        }

    }

    // // Draw each entity as a square
    // for (int i = 0; i < ME_MAX_ENTITIES; i++)
    // {
    //     if (!g->mainScene->alive[i]) continue;
    //     if (!g->mainScene->has_transform[i]) continue;
    //     if (!g->mainScene->has_mesh[i]) continue;

    //     me_mesh *m = me_scene_get_mesh(g->mainScene, i);

    //     for (int i = 0; i < m->vertCount; i += 3)
    //     {
    //         mr_renderer_render_triangle(&g->renderer, 
    //             (int)m->verts[i * 3].x, (int)m->verts[i * 3].y,
    //             (int)m->verts[i * 3 + 1].x, (int)m->verts[i * 3 + 1].y,
    //             (int)m->verts[i * 3 + 2].x, (int)m->verts[i * 3 + 2].y,
    //             mr_rgb(255, 0, 255)
    //         );
    //     }
    // }
}