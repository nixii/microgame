
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
}

// destroy a whole scene
void scene_destroy(scene *s) {
    free(s);
}

// get a transform
transform *get_transform(scene *s, entity e) {
    if (!s->alive[e]) return NULL;
    return &s->transforms[e];
}

// render a scene!
void scene_render(scene *s, renderer *r) {

    // render each entity
    for (entity id = 0; id < MAX_ENTITIES; id++) {
        if (!s->alive[id]) continue;

        // get the position
        vec3 pos = get_transform(s, id)->pos;

        // render the pos
        renderer_render_square(r, pos.x, pos.y, pos.z, pos.z, rgb(255, 0, 255));
    }
}