
#include <stdlib.h>
#include <string.h>
#include "microengine/scene.h"

// create a new empty scene
me_scene *me_scene_new()
{
    
    // allocate the scene to zeros
    me_scene *s = calloc(1, sizeof(me_scene));

    // set the free entity id stack
    for (s->free_top = 0; s->free_top < ME_MAX_ENTITIES; s->free_top++)
    {
        s->free_stack[s->free_top] = s->free_top;
    }

    // the free top index is one less
    s->free_top = ME_MAX_ENTITIES - 1;

    // return the scene
    return s;
}

// spawn a brand new entity
me_entity me_scene_spawn(me_scene *s)
{
    if (s->free_top == 0)
        return ME_NIL_ENTITY;

    me_entity id = s->free_stack[--s->free_top];

    s->alive[id] = 1;

    return id;
}

// destroy an entity
void me_scene_despawn(me_scene *s, me_entity e)
{
    if (!s->alive[e]) return;

    s->alive[e] = 0;

    s->free_stack[s->free_top++] = e;
}

// destroy the whole scene
void me_scene_destroy(me_scene *s)
{
    free(s);
}

// generate the functions for attaching components
#define X(name, type)\
    void me_scene_attach_##name##(me_scene *s, entity e, type t)\
    {\
        s->name##_has[e] = 1;\
        s->name##_components[e] = t;\
    }\
    void me_scene_detach_##name##(me_scene *s, entity e)\
    {\
        s->name##_has[e] = 0;\
    }
    ME_COMPONENTS
#undef X