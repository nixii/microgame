
#ifndef MG_COMPONENTS_X_H
#define MG_COMPONENTS_X_H

#include "microgame/engine/components/collider.h"
#include "microgame/engine/components/velocity.h"
#include "microgame/engine/components/mesh.h"

#define X_COMPONENTS\
    X(mesh, mesh)\
    X(collider, collider)\
    X(velocity, velocity)

#endif // MG_COMPONENTS_X_H