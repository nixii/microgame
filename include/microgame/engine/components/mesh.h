
#ifndef MG_COMPONENTS_MESH_H
#define MG_COMPONENTS_MESH_H

#include "microgame/util/math.h"

typedef struct mesh {
    int numVerts;
    vec3 *verts;
} mesh;

mesh mesh_new(int numVerts, ...);
void mesh_destroy(mesh *m);

#endif // MG_COMPONENTS_MESH_H