
#ifndef MG_SCENE_H
#define MG_SCENE_H

#include "microgame/engine/ui/container.h"
#include "microgame/engine/components/x.h"
#include "microgame/renderer/renderer.h"
#include "microgame/util/transform.h"
#include "microgame/util/triangle.h"
#include "microgame/engine/camera.h"
#include "microgame/engine/entity.h"
#include <stdint.h>

#define MAX_ENTITIES 4096
#define INIT_TRIANGLE_COUNT 128

// scene type
typedef struct scene {
    
    // entity stuff
    uint8_t alive[MAX_ENTITIES];
    entity freeIds[MAX_ENTITIES];
    entity parents[MAX_ENTITIES];
    int lastFree;
    transform transforms[MAX_ENTITIES];

    // optimize by storing global matrices
    uint8_t hasGlobalMat[MAX_ENTITIES];
    mat4 globalMats[MAX_ENTITIES];

    // triangles to render
    int triangleBufferSize;
    triangle *triangleBuffer;

    // for rendering
    camera camera;

    // the entire UI tree
    ui_container *uiRoot;

    // load different component data
#define X(name, type)\
    uint8_t has_##name[MAX_ENTITIES];\
    type name##_components[MAX_ENTITIES];
    X_COMPONENTS
#undef X
} scene;

/*
 * scene functions
*/

// allocate a brand new scene. this sets all the arrays to default values.
scene *scene_new();

// spawn a new entity. if there is no space this returns NIL_ENTITY
entity scene_spawn(scene *s);

// render the scene fully! (not sure if the scene should own this but it does!)
void scene_render(
    scene *s, 
    renderer *r, 
    vec2 mousePos, 
    int mouseLeftDown,
    float dt);

// despawn an entity when you are done with it; frees all the components
void scene_despawn(scene *s, entity e);

// destroy the entire scene, despawning entities as well.
void scene_destroy(scene *s);

// get the transform of an entity
transform *get_transform(scene *s, entity e);

// get the parent of an entity
entity get_parent(scene *s, entity child);

// set the parent of an entity
void set_parent(scene *s, entity child, entity parent);

// remove the parent of an entity
void remove_parent(scene *s, entity child);

// set the ui root of the scene
void scene_set_ui_root(scene *s, ui_container *c);

// get the ui root of the scene, maybe NULL
ui_container *scene_get_ui_root(scene *s);

// component functions on the scene
#define X(name, type)\
    int scene_has_##name(scene *s, entity e);\
    type *scene_get_##name(scene *s, entity e);\
    type *scene_attach_##name(scene *s, entity e, type c);\
    void scene_detach_##name(scene *s, entity e);
X_COMPONENTS
#undef X

#endif // MG_SCENE_H