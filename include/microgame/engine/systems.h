
#ifndef MG_SYSTEMS_H
#define MG_SYSTEMS_H

#include "microgame/engine/entity.h"

// for errors
#define COLLISION_EPSILON 0.001

// just forward-declare the scene
typedef struct scene scene;

// handle collision system
void collision_system_update(scene *s, entity first, float dt);

// handle velocity system
void velocity_system_update(scene *s, entity e, float dt);

#endif // MG_SYSTEMS_H