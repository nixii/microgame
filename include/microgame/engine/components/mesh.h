
#ifndef MG_COMPONENTS_MESH_H
#define MG_COMPONENTS_MESH_H

#include "microgame/util/math.h"
#include "microgame/util/color.h"

typedef struct mesh {
    int numVertices;
    vec3 *vertices;
    int numIndices;
    int *indices;
    color color;
} mesh;

mesh mesh_new(color c, int numVerts, vec3 *verts, int numIndices, int *indices);
void mesh_destroy(mesh *m);

#endif // MG_COMPONENTS_MESH_H