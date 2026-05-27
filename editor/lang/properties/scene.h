
#ifndef MS_INTERP_PROPERTIES_SCENE_H
#define MS_INTERP_PROPERTIES_SCENE_H

#include <microgame/microgame.h>
#include "../data.h"

// get/set a property on a scene
// properties:
//  - light_direction <Vec3>
void ms_interpreter_scene_set_property(scene *s, const char *propName, ms_data value);
ms_data ms_interpreter_scene_get_property(scene *s, const char *propName);

#endif // MS_INTERP_PROPERTIES_SCENE_H