
#include "microgame/engine/components/mesh.h"
#include <stdlib.h>
#include <stdarg.h>

mesh mesh_new(int numVerts, ...) {

    // create the mesh
    mesh m = { 0 };
    m.numVerts = numVerts;
    m.verts = malloc(numVerts * sizeof(vec3));

    // load the triangles from varargs
    va_list args;
    va_start(args, numVerts);

    // add each point
    for (int i = 0; i < numVerts; i++)
    {
        m.verts[i] = va_arg(args, vec3);
    }

    // all done!
    va_end(args);
    return m;
}

void mesh_destroy(mesh *m) {
    free(m->verts);
}