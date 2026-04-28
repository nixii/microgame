
#include "microgame/engine/components/mesh.h"
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

    // allocate and copy vertices and indices
    m.vertices = malloc(numVerts * sizeof(vec3));
    memcpy(m.vertices, verts, numVerts * sizeof(vec3));
    m.indices = malloc(numIndices * sizeof(vec3));
    memcpy(m.indices, indices, numIndices * sizeof(vec3));
    return m;
}

// load a mesh from a .obj file
mesh mesh_from_obj(color c, char *objFilePath) {

    // create the mesh
    mesh m = { 0 };

    // load the file
    FILE *objFile = fopen(objFilePath, "r");

    // error if it doesn't exist
    if (objFile == NULL) {
        errno = 1;
        printf("oh nooo\n");
        return m;
    }

    // how much is read
    char readBytes[OBJ_READ_BUFFER];
    char *endPoint;

    // state info
    int isComment = 0;
    char command[2];

    // read into the read bytes
    while ((endPoint = fgets(readBytes, sizeof(readBytes), objFile)) != NULL) {
        
        // print out the characters
        for (int i = 0; i < sizeof(readBytes); i++) {

            // skip if comment or null
            if (readBytes[i] == '\0') break;
            if (readBytes[i] == '#') break;
            
            // TODO: find the vertices
        }
    }

    // exit
exitnow:

    // close the file
    fclose(objFile);

    // return the mesh
    return m;
}

// destroy a mesh
void mesh_destroy(mesh *m) {
    free(m->vertices);
    free(m->indices);
    m->numIndices = 0;
    m->numVertices = 0;
}