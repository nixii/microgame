
#ifndef MS_INTERP_PROPERTIES_COMPONENT_VELOCITY_H
#define MS_INTERP_PROPERTIES_COMPONENT_VELOCITY_H

#include <microgame/microgame.h>
#include "../data.h"

// get/set a property on a velocity
// properties:
//  - velocity <Vec3>
void ms_interpreter_component_velocity_set_property(velocity *v, const char *propName, ms_data value);
ms_data ms_interpreter_component_velocity_get_property(velocity *v, const char *propName);

#endif // MS_INTERP_PROPERTIES_COMPONENT_VELOCITY_H