
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

        // skip commented lines
        if (readBytes[0] == '#')
            continue;
        
        // the partially loaded objects
        int param = 0;
        float coordinates[4];
        int numCoordinates = 0;
        
        // expect the command
        int i = 0;
        while (i < 2 && readBytes[i] != ' ') {
            command[i] = readBytes[i];
            i++;
        }
        
        // print the command
        printf("%s, ", command);

        // parse vertices
        if (strcmp(command, "v") == 0) {

            // pointers for traversing the string
            char *next = readBytes + i;
            char *end = NULL;

            // repeat while there are still points
            while (1) {

                // get the next float
                float coordinate = strtof(next, &end);

                // skip if you must
                if (end == next) break;

                // add the coordinate and go to the next
                next = end + 1;
                coordinates[numCoordinates++] = coordinate;
            }
            
            // TODO: make the points get loaded into the mesh (w included)
            if (numCoordinates < 3) break;

            // add the coordinates
            float w = numCoordinates == 4 ? coordinates[3] : 1.0;
            vec3 v = vec3_new(coordinates[0] / w, coordinates[1] / w, coordinates[2] / w);

            // make the vertex larger if needed
            if (numVertices >= verticesCap) {
                verticesCap *= 2;
                vertices = realloc(vertices, sizeof(vec3) * verticesCap);
            }

            // set the vertex
            vertices[numVertices++] = v;
        }

        // TODO: parse faces
        if (strcmp(command, "f") == 0) {

        }

        // reset the command
        command[0] = 0;
        command[1] = 0;
        command[2] = 0;
    }

    // scale down the vertices
    vertices = realloc(vertices, numVertices * sizeof(vec3));
    m.vertices = vertices;
    m.numVertices = numVertices;

    // scale down the indices
    indices = realloc(indices, numIndices * sizeof(int));
    m.indices = indices;
    m.numIndices = numIndices;

    // return the model
    return m;

    // a failure
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