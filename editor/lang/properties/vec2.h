
#ifndef MS_INTERP_PROPERTIES_VEC2_H
#define MS_INTERP_PROPERTIES_VEC2_H

#include <microgame/microgame.h>
#include "../data.h"

// get/set vec2 properties
// properties:
//  - x <float>
//  - y <float>
void ms_interpreter_vec2_set_property(vec2 *v, const char *propName, ms_data value);
ms_data ms_interpreter_vec2_get_property(vec2 *v, const char *propName);

#endif // MS_INTERP_PROPERTIES_VEC2_H