
#include "microengine/component/mesh.h"
#include "microengine/math.h"
#include <stdlib.h>
#include <stdarg.h>

// make a mesh with points
me_mesh me_mesh_from(int triangleCount, ...)
{

    // create the mesh
    me_mesh m = { 0 };
    m.triCount = triangleCount;
    m.points = malloc(triangleCount * 3 * sizeof(me_vec3));

    // load the triangles from varargs
    va_list args;
    va_start(args, triangleCount * 3);
    
    // add each point
    for (int i = 0; i < triangleCount * 3; i++)
    {
        m.points[i] = va_arg(args, me_vec3);
    }

    // all done!
    va_end(args);
    return m;
}