
#include "microgame/engine/components/mesh.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

mesh mesh_new(color c, int numVerts, vec3 *verts, int numIndices, int *indices) {

    // create the mesh
    mesh m = { 0 };
    m.numVertices = numVerts;
    m.numIndices = numIndices;
    m.color = c;
    m.vertices = malloc(numVerts * sizeof(vec3));
    memcpy(m.vertices, verts, numVerts * sizeof(vec3));
    m.indices = malloc(numIndices * sizeof(vec3));
    memcpy(m.indices, indices, numIndices * sizeof(vec3));
    return m;
}

void mesh_destroy(mesh *m) {
    free(m->vertices);
    free(m->indices);
    m->numIndices = 0;
    m->numVertices = 0;
}