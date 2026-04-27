
#ifndef MG_SCENE_H
#define MG_SCENE_H

#include "microgame/util/transform.h"
#include "microgame/util/triangle.h"
#include "microgame/engine/camera.h"
#include "microgame/renderer/renderer.h"
#include "microgame/engine/entity.h"
#include "microgame/engine/components/x.h"
#include <stdint.h>

#define MAX_ENTITIES 4096
#define INIT_TRIANGLE_COUNT 128

// scene type
typedef struct scene {
    
    // entity stuff
    uint8_t alive[MAX_ENTITIES];
    entity freeIds[MAX_ENTITIES];
    transform transforms[MAX_ENTITIES];
    int lastFree;

    // triangles to render
    int triangleBufferSize;
    triangle *triangleBuffer;

    // for rendering
    camera camera;

    // load different component data
#define X(name, type)\
    uint8_t has_##name[MAX_ENTITIES];\
    type name##_components[MAX_ENTITIES];
    X_COMPONENTS
#undef X
} scene;

// scene functions
scene *scene_new();
entity scene_spawn(scene *s);
void scene_render(scene *s, renderer *r);
void scene_despawn(scene *s, entity e);
void scene_destroy(scene *s);
transform *get_transform(scene *s, entity e);

// component functions on the scene
#define X(name, type)\
    int scene_has_##name(scene *s, entity e);\
    type *scene_get_##name(scene *s, entity e);\
    type *scene_attach_##name(scene *s, entity e, type c);\
    void scene_detach_##name(scene *s, entity e);
X_COMPONENTS
#undef X

#endif // MG_SCENE_H