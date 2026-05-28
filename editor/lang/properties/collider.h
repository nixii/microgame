
#ifndef MS_INTERP_PROPERTIES_COMPONENT_COLLIDER_H
#define MS_INTERP_PROPERTIES_COMPONENT_COLLIDER_H

#include <microgame/microgame.h>
#include "../data.h"

// get/set a property on a collider
// properties:
//  - size <Vec3>
void ms_interpreter_component_collider_set_property(scene *s, entity e, const char *propName, ms_data value);
ms_data ms_interpreter_component_collider_get_property(scene *s, entity e, const char *propName);

#endif // MS_INTERP_PROPERTIES_COMPONENT_COLLIDER_H