
#include "microengine/component/mesh.h"
#include "microengine/math.h"
#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>

// make a mesh with points
me_mesh me_mesh_from(int vertCount, ...)
{

    // create the mesh
    me_mesh m = { 0 };
    m.vertCount = vertCount;
    m.verts = malloc(vertCount * sizeof(me_vec3));

    // load the triangles from varargs
    va_list args;
    va_start(args, vertCount);
    
    // add each point
    for (int i = 0; i < vertCount; i++)
    {
        m.verts[i] = va_arg(args, me_vec3);
    }

    // all done!
    va_end(args);
    return m;
}

void me_mesh_destroy(me_mesh *mesh)
{
    free(mesh->verts);
    mesh->verts = NULL;
    mesh->vertCount = 0;
}