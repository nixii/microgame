
#ifndef MICROENGINE_MESH_H
#define MICROENGINE_MESH_H

#include "microengine/math.h"

typedef struct me_mesh {
    me_vec3 *points;
    int triCount;
} me_mesh;

// make a mesh with points
me_mesh me_mesh_from(int triangleCount, ...);

#ifndef MICROENGINE_NOSTRIP
    #define mesh me_mesh
    #define mesh_from me_mesh_from
#endif

#endif // MICROENGINE_MESH_H