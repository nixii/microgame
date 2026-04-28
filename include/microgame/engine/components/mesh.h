
#ifndef MG_COMPONENTS_MESH_H
#define MG_COMPONENTS_MESH_H

#include "microgame/util/math.h"
#include "microgame/util/color.h"

#define OBJ_READ_BUFFER 4096

typedef struct mesh {
    int numVertices;
    vec3 *vertices;
    int numIndices;
    int *indices;
    color color;
} mesh;

mesh mesh_new(color c, int numVerts, vec3 *verts, int numIndices, int *indices);
mesh mesh_from_obj(color c, char *objFile);
void mesh_destroy(mesh *m);

#endif // MG_COMPONENTS_MESH_H