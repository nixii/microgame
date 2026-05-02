
#ifndef MG_SYSTEMS_H
#define MG_SYSTEMS_H

// just forward-declare the scene
typedef struct scene scene;

// handle collision system
void collision_system_update(scene *s, int maxEntities, float dt);

#endif // MG_SYSTEMS_H