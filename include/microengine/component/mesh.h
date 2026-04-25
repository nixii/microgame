
#ifndef MICROENGINE_MESH_H
#define v

typedef struct me_mesh {
    float *points;
    int triCount;
} me_mesh;

// make a mesh with points
me_mesh me_mesh_from(int triangleCount, ...);

#ifndef MICROENGINE_NOSTRIP
    #define mesh me_mesh
    #define mesh_from me_mesh_from
#endif

#endif // MICROENGINE_MESH_H