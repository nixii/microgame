
#ifndef MS_INTERP_PROPERTIES_COMPONENT_MESH_H
#define MS_INTERP_PROPERTIES_COMPONENT_MESH_H

#include <microgame/microgame.h>
#include "../data.h"

// get/set a property on a mesh
// properties:
//  - color <Vec3>
void ms_interpreter_component_mesh_set_property(mesh *m, const char *propName, ms_data value);
ms_data ms_interpreter_component_mesh_get_property(mesh *m, const char *propName);

#endif // MS_INTERP_PROPERTIES_COMPONENT_MESH_H