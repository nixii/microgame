
#ifndef MS_INTERP_PROPERTIES_VEC3_H
#define MS_INTERP_PROPERTIES_VEC3_H

#include <microgame/microgame.h>
#include "../data.h"

// get/set vec3 properties
// properties:
//  - x <float>
//  - y <float>
//  - z <float>
void ms_interpreter_vec3_set_property(vec3 *v, const char *propName, ms_data value);
ms_data ms_interpreter_vec3_get_property(vec3 *v, const char *propName);

#endif // MS_INTERP_PROPERTIES_VEC3_H