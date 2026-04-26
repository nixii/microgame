
#ifndef MG_SCENE_H
#define MG_SCENE_H

#include "microgame/util/transform.h"
#include "microgame/engine/camera.h"
#include "microgame/renderer/renderer.h"
#include "microgame/engine/entity.h"
#include <stdint.h>

#define MAX_ENTITIES 4096

// scene type
typedef struct scene {
    
    // entity stuff
    uint8_t alive[MAX_ENTITIES];
    entity freeIds[MAX_ENTITIES];
    transform transforms[MAX_ENTITIES];
    int lastFree;

    // for rendering
    camera camera;
} scene;

// scene functions
scene *scene_new();
entity scene_spawn(scene *s);
void scene_render(scene *s, renderer *r);
void scene_despawn(scene *s, entity e);
void scene_destroy(scene *s);
transform *get_transform(scene *s, entity e);

#endif // MG_SCENE_H