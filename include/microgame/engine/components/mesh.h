
#ifndef MG_COMPONENTS_MESH_H
#define MG_COMPONENTS_MESH_H

#include "microgame/util/math.h"
#include "microgame/util/color.h"
#include "microgame/engine/resource/mesh.h"

typedef struct mesh {
    int numVertices;
    vec3 *vertices;
    int numIndices;
    int *indices;
    color color;
    int fromResource;
} mesh;

mesh mesh_new(color c, int numVerts, vec3 *verts, int numIndices, int *indices);
mesh mesh_from_resource(color color, mesh_resource r);
void mesh_destroy(mesh *m);

#endif // MG_COMPONENTS_MESH_H