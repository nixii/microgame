
#ifndef MICROENGINE_SCENE_H
#define MICROENGINE_SCENE_H

#include "microengine/entity.h"
#include "microengine/components.h"
#include <stdint.h>

typedef struct me_scene {
    uint8_t alive[ME_MAX_ENTITIES];

    // add components X macro
#define X(name, type)\
    uint8_t name##_has[ME_MAX_ENTITIES];\
    type name##_components[ME_MAX_ENTITIES];

    ME_COMPONENTS

#undef X
} me_scene;

// spawn a brand new entity
me_entity me_scene_spawn();

// destroy an entity
void me_scene_destroy(me_entity e);

#endif // MICROENGINE_SCENE_H