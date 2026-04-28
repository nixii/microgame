
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

// TODO: move this out into an "MeshResource" struct
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
    char numberBuffer[OBJ_READ_BUFFER];
    int numberBufferLen = 0;
    char *endPoint;

    // state info
    char command[3] = {0, 0, 0};

    // the arrays that will store the objects eventually
    vec3 *vertices = malloc(64 * sizeof(vec3));
    int numVertices = 0, verticesCap = 64;
    int *indices = malloc(64 * sizeof(int));
    int numIndices = 0, indicesCap = 64;

    // read into the read bytes
    while ((endPoint = fgets(readBytes, sizeof(readBytes), objFile)) != NULL) {

        // the partially loaded objects
        int param = 0;
        float points[4];
        
        // expect the command
        int i = 0;
        while (i < 2 && readBytes[i] != " ") {
            command[i] = readBytes[i];
            i++;
        }
        
        // print the command
        printf("%s, ", command);

        // parse vertices
        if (strcmp(command, "v ") == 0) {
            char *next = readBytes;
            // TODO: read every float it can and store it in the points.
            // TODO: make the points get loaded into the mesh (w included)

            printf("\n");
        }

        // TODO: parse faces

        // reset the command
        command[0] = 0;
        command[1] = 0;
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