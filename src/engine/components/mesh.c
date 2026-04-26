
#include "microgame/engine/components/mesh.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

mesh mesh_new(color c, int numVerts, vec3 *verts) {

    // create the mesh
    mesh m = { 0 };
    m.numVerts = numVerts;
    m.color = c;
    m.verts = malloc(numVerts * sizeof(vec3));
    memcpy(m.verts, verts, numVerts * sizeof(vec3));
    return m;
}

void mesh_destroy(mesh *m) {
    free(m->verts);
}