
#include "microgame/renderer/renderer.h"
#include "microgame/engine/scene.h"
#include "microgame/util/color.h"
#include <stdlib.h>
#include <stdio.h>

// create a new scene
scene *scene_new() {
    scene *s = calloc(1, sizeof(scene));
    s->camera = camera_new();

    // load the free IDs
    for (s->lastFree = 0; s->lastFree < MAX_ENTITIES; s->lastFree++) {
        s->freeIds[s->lastFree] = s->lastFree;
    }
    s->lastFree = MAX_ENTITIES - 1;

    return s;
}

// spawn an entity
entity scene_spawn(scene *s) {
    if (s->lastFree < 0)
        return NIL_ENTITY;
    entity id = s->freeIds[s->lastFree--];
    s->alive[id] = 1;
    s->transforms[id] = transform_blank();
    return id;
}

// despawn an entity
void scene_despawn(scene *s, entity e) {
    if (s->alive[e] != 1)
        return;
    s->alive[e] = 0;
    s->freeIds[++s->lastFree] = e;

    // destroy any components
#define X(name, type)\
    scene_detach_##name(s, e);
    X_COMPONENTS
#undef X
}

// destroy a whole scene
void scene_destroy(scene *s) {

    // destroy components
    for (entity e = 0; e < MAX_ENTITIES; e++) {
        if (!s->alive[e]) continue;
    #define X(name, type)\
        scene_detach_##name(s, e);
        X_COMPONENTS
    #undef X
    }

    // destroy the scene
    free(s);
}

// get a transform
transform *get_transform(scene *s, entity e) {
    if (!s->alive[e]) return NULL;
    return &s->transforms[e];
}

// component functions
#define X(name, type) \
    int scene_has_##name(scene *s, entity e) { \
        return s->has_##name[e]; \
    } \
    type *scene_get_##name(scene *s, entity e) { \
        if (!scene_has_##name(s, e)) return NULL; \
        return &s->name##_components[e]; \
    } \
    type *scene_attach_##name(scene *s, entity e, type c) { \
        s->has_##name[e] = 1; \
        s->name##_components[e] = c; \
        return &s->name##_components[e]; \
    } \
    void scene_detach_##name(scene *s, entity e) { \
        s->has_##name[e] = 0; \
        name##_destroy(&s->name##_components[e]); \
    }
X_COMPONENTS
#undef X

// render a scene!
void scene_render(scene *s, renderer *r) {

    // render each entity
    for (entity id = 0; id < MAX_ENTITIES; id++) {
        if (!s->alive[id]) continue;

        // get the position
        vec3 pos = get_transform(s, id)->pos;

        // mesh rendering
        if (scene_has_mesh(s, id)) {
            mesh *m = scene_get_mesh(s, id);

            // render!
            for (int i = 0; i < m->numVerts; i += 3) {

                // normal
                vec3 normal = vec3_normal(vec3_cross(vec3_sub(m->verts[i + 1], m->verts[i]), vec3_sub(m->verts[i + 2], m->verts[i])));
                float lighting = (-vec3_dot(normal, vec3_new(1, 0, 0)) + 1) / 2;

                // points
                vec2 v1 = camera_project_point(&s->camera, vec3_add(m->verts[i], pos), r->width, r->height);
                vec2 v2 = camera_project_point(&s->camera, vec3_add(m->verts[i+1], pos), r->width, r->height);
                vec2 v3 = camera_project_point(&s->camera, vec3_add(m->verts[i + 2], pos), r->width, r->height);

                // render the triangle
                renderer_render_triangle(r, v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, rgb_mix(m->color, rgb(lighting * 256, lighting * 256, lighting * 256)));
            }
        }
    }
}