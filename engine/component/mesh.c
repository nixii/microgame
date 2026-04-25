
#include "microengine/component/mesh.h"
#include <stdarg.h>

// make a mesh with points
me_mesh me_mesh_from(int triangleCount, ...)
{

    // create the mesh
    me_mesh m = { 0 };
    m.triCount = triangleCount;
    m.points = malloc(triangleCount * 3 * sizeof(float));

    // load the triangles from varargs
    va_list args;
    va_start(args, triangleCount * 3)
    
    // add each point
    for (int i = 0; i < triangleCount * 3; i++)
    {
        points[i] = va_arg(args, float);
    }

    // all done!
    va_end(args);
    return m;
}