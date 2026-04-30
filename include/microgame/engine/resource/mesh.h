
#ifndef MG_MESH_RESOURCE_H
#define MG_MESH_RESOURCE_H

#include "microgame/util/math.h"

// the maximum amount of bytes a line in the object file may be
#define OBJ_READ_BUFFER 4096

// a mesh resource stores vertices and indices
// no color or texture or anything else
typedef struct mesh_resource {
    vec3 *vertices;
    int numVertices;
    int *indices;
    int numIndices;
} mesh_resource;

/*
 * mesh resource functions
*/

// load a mesh resource from a .obj file
mesh_resource mesh_resource_from_obj(const char *path);

// destroy a mesh resource when done
void mesh_resource_destroy(mesh_resource *r);

#endif // MG_MESH_RESOURCE_H