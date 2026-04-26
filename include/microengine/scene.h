
#ifndef MICROENGINE_SCENE_H
#define MICROENGINE_SCENE_H

#include "microengine/entity.h"
#include "microengine/camera.h"
#include "microengine/components.h"
#include <stdint.h>

typedef struct me_scene {
    uint8_t alive[ME_MAX_ENTITIES];
    int free_stack[ME_MAX_ENTITIES];
    int free_top;

    // add components X macro
    #define X(name, type)\
        uint8_t has_##name[ME_MAX_ENTITIES];\
        type name##_components[ME_MAX_ENTITIES];
        ME_COMPONENTS
    #undef X

    // add the camera
    me_camera cam;
} me_scene;

// generate the functions for attaching components
#define X(name, type)\
    void me_scene_attach_##name(me_scene *s, entity e, type t);\
    void me_scene_detach_##name(me_scene *s, entity e);\
    int me_scene_has_##name(me_scene *s, entity e);\
    type *me_scene_get_##name(me_scene *s, entity e);
    ME_COMPONENTS
#undef X

// create a new empty scene
me_scene *me_scene_new();

// spawn a brand new entity
me_entity me_scene_spawn(me_scene *s);

// destroy an entity
void me_scene_despawn(me_scene *s, me_entity e);

// destroy the whole scene
void me_scene_destroy(me_scene *s);

// alias everything
#ifndef MICROENGINE_NOSTRIP
    #define scene me_scene
    #define scene_new me_scene_new
    #define scene_spawn me_scene_spawn
    #define scene_despawn me_scene_despawn
    #define scene_destroy me_scene_destroy
#endif

#endif // MICROENGINE_SCENE_H