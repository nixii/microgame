
#include "microgame/engine/components/mesh.h"
#include "microgame/engine/resource/mesh.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

// create a new mesh
mesh mesh_new(color c, int numVerts, vec3 *verts, int numIndices, int *indices) {

    // create the mesh
    mesh m = { 0 };
    m.numVertices = numVerts;
    m.numIndices = numIndices;
    m.color = c;
    m.fromResource = 0;

    // allocate and copy vertices and indices
    m.vertices = malloc(numVerts * sizeof(vec3));
    memcpy(m.vertices, verts, numVerts * sizeof(vec3));
    m.indices = malloc(numIndices * sizeof(vec3));
    memcpy(m.indices, indices, numIndices * sizeof(vec3));
    return m;
}

// TODO: move this out into an "MeshResource" struct
// load a mesh from a .obj file
mesh mesh_from_resource(color color, mesh_resource r) {

    // create the mesh
    mesh m;

    // load the values from the resource
    m.numVertices = r.numVertices;
    m.vertices = r.vertices;
    m.numIndices = r.numIndices;
    m.indices = r.indices;

    // set the color of the mesh
    m.color = color;

    // it is from a resource
    m.fromResource = 1;

    // hand the mesh out
    return m;
}

// destroy a mesh
void mesh_destroy(mesh *m) {

    // free the pointers if they are allocated by this mesh
    if (!m->fromResource) {
        free(m->vertices);
        free(m->indices);
    }

    // no vertices here!
    m->numIndices = 0;
    m->numVertices = 0;
}