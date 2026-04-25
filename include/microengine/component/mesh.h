
#ifndef MICROENGINE_MESH_H
#define MICROENGINE_MESH_H

#include "microengine/math.h"

typedef struct me_mesh {
    me_vec3 *verts;
    int vertCount;
} me_mesh;

// make a mesh with points
me_mesh me_mesh_from(int vertCount, ...);

// destroy a mesh
void me_mesh_destroy(me_mesh *m);

#ifndef MICROENGINE_NOSTRIP
    #define mesh me_mesh
    #define mesh_from me_mesh_from
    #define mesh_destroy me_mesh_destroy
#endif

#endif // MICROENGINE_MESH_H