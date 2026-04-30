
#include "microgame/renderer/renderer.h"
#include "microgame/engine/scene.h"
#include "microgame/util/color.h"
#include <stdlib.h>
#include <stdio.h>

// create a new scene
scene *scene_new() {
    scene *s = calloc(1, sizeof(scene));
    s->camera = camera_new();

    // vertices ready to render
    s->triangleBufferSize = INIT_TRIANGLE_COUNT;
    s->triangleBuffer = (triangle *)malloc(sizeof(triangle) * s->triangleBufferSize);

    // load the free IDs
    for (s->lastFree = 0; s->lastFree < MAX_ENTITIES; s->lastFree++) {
        s->freeIds[s->lastFree] = s->lastFree;
        s->parents[s->lastFree] = NIL_ENTITY;
    }
    s->lastFree = MAX_ENTITIES - 1;

    return s;
}

// spawn an entity
entity scene_spawn(scene *s) {

    // if there is no room then don't spawn a new entity
    if (s->lastFree < 0)
        return NIL_ENTITY;

    // get the next id and decrement the amount free
    entity id = s->freeIds[s->lastFree--];

    // alive is true
    s->alive[id] = 1;

    // create the base objects
    s->transforms[id] = transform_blank();
    s->parents[id] = NIL_ENTITY;

    // return the entity's ID
    return id;
}

// despawn an entity
void scene_despawn(scene *s, entity e) {
    if (s->alive[e] != 1)
        return;
    s->alive[e] = 0;
    s->freeIds[++s->lastFree] = e;
    s->parents[s->lastFree] = NIL_ENTITY;

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
    free(s->triangleBuffer);
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

    // list of all triangles
    int numTris = 0;

    // for each entity
    for (entity e = 0; e < MAX_ENTITIES; e++) {
        if (!s->alive[e]) continue;

        // load its transform
        transform *t = get_transform(s, e);

        // get the mesh
        mesh *m = scene_get_mesh(s, e);

        // for each triangle
        for (int i = 0; i < m->numIndices; i += 3) {

            // global locs
            vec3 v1 = vec3_add(m->vertices[m->indices[i]], t->pos);
            vec3 v2 = vec3_add(m->vertices[m->indices[i + 1]], t->pos);
            vec3 v3 = vec3_add(m->vertices[m->indices[i + 2]], t->pos);
            
            // translate and clip the triangles
            camera_translation_result tris = camera_translate_triangle(&s->camera, v1, v2, v3);
            if (tris.numTriangles == 0) continue;

            // color the triangles
            for (int i = 0; i < tris.numTriangles; i++)
                tris.triangles[i].color = m->color;

            // expand if the amount of triangles to add is too many
            if (numTris + tris.numTriangles >= s->triangleBufferSize) {
                s->triangleBufferSize *= 2;
                s->triangleBuffer = realloc(s->triangleBuffer, sizeof(triangle) * s->triangleBufferSize);
            }

            // add it in
            for (int i = 0; i < tris.numTriangles; i++)
                s->triangleBuffer[numTris++] = tris.triangles[i];
        }
    }
    
    // draw each triangle!
    for (int i = 0; i < numTris; i++) {
        triangle t = s->triangleBuffer[i];

        vec3 v1 = camera_project_point(&s->camera, t.a, r->width, r->height);
        vec3 v2 = camera_project_point(&s->camera, t.b, r->width, r->height);
        vec3 v3 = camera_project_point(&s->camera, t.c, r->width, r->height);

        // skip behind the camera
        if ((v1.x == v1.y && v1.x == -1) ||
            (v2.x == v2.y && v2.x == -1) ||
            (v3.x == v3.y && v3.x == -1))
            continue;

        // render the triangle
        float lighting = (vec3_dot(t.normal, vec3_new(0, 1, 0)) + 1) / 5.0;
        renderer_render_triangle(r, v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z, rgb_mix(t.color, rgb(lighting * 256, lighting * 256, lighting * 256)));
    }
}   

// get the parent of an entity
entity get_parent(scene *s, entity child) {
    return s->parents[child];
}

// set the parent of an entity
void set_parent(scene *s, entity child, entity parent) {
    s->parents[child] = parent;
}