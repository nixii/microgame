
#ifndef MG_COMPONENTS_MESH_H
#define MG_COMPONENTS_MESH_H

#include "microgame/util/math.h"
#include "microgame/util/color.h"

typedef struct mesh {
    int numVerts;
    vec3 *verts;
    color color;
} mesh;

mesh mesh_new(color c, int numVerts, vec3 *verts);
void mesh_destroy(mesh *m);

#endif // MG_COMPONENTS_MESH_H