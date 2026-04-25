
#ifndef MICROENGINE_COMPONENTS_H
#define MICROENGINE_COMPONENTS_H

#include "microengine/component/transform.h"
#include "microengine/component/mesh.h"

// X-macro for components.
#define ME_COMPONENTS \
    X(transform, me_transform) \
    X(mesh, me_mesh)

#endif // MICROENGINE_COMPONENTS_H