
#ifndef MS_INTERP_PROPERTIES_ENTITY_H
#define MS_INTERP_PROPERTIES_ENTITY_H

#include <microgame/microgame.h>
#include "../data.h"

// get/set a property on an entity
// properties:
//  - position <Vec3>
//  - rotation <Vec3>
//  - scale <Vec3>
// TODO:
//  - guid <string>
void ms_interpreter_entity_set_property(scene *s, entity e, const char *propName, ms_data value);
ms_data ms_interpreter_entity_get_property(scene *s, entity e, const char *propName);

#endif // MS_INTERP_PROPERTIES_ENTITY_H