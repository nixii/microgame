
#include "microgame/engine/resource/mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

mesh_resource mesh_resource_from_obj(const char *objFilePath) {
    
    // create the mesh
    mesh_resource m = { 0 };

    // load the file
    FILE *objFile = fopen(objFilePath, "r");

    // error if it doesn't exist
    if (objFile == NULL) {
        errno = 1;
        printf("file doesn't exist: %s\n", objFilePath);
        return m;
    }

    // how much is read
    char readBytes[OBJ_READ_BUFFER];
    char *endPoint;

    // state info
    char command[3] = {0, 0, 0};

    // the arrays that will store the objects eventually
    int numVertices = 0, verticesCap = 64;
    vec3 *vertices = malloc(verticesCap * sizeof(vec3));
    int numIndices = 0, indicesCap = 64 * 3;
    int *indices = malloc(indicesCap * sizeof(int));

    // read into the read bytes
    while ((endPoint = fgets(readBytes, sizeof(readBytes), objFile)) != NULL) {

        // skip commented lines
        if (readBytes[0] == '#')
            continue;
        
        // the partially loaded objects
        float coordinates[4];
        int numCoordinates = 0;
        
        // expect the command
        int i = 0;
        while (i < 2 && readBytes[i] != ' ') {
            command[i] = readBytes[i];
            i++;
        }

        // parse vertices
        if (strcmp(command, "v") == 0) {

            // pointers for traversing the string
            char *next = readBytes + i;
            char *end = NULL;

            // TODO: add safeguards
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

        if (strcmp(command, "f") == 0) {

            // store the first and previous for fans
            int idxFirst = -1;
            int idxPrevious = -1;

            // point to the first element that isn't the space
            char *next = readBytes + i + 1;
            char *end = NULL;

            // go through
            while (*next != '\0') {
                int v = (int)strtol(next, &end, 10);
                if (next == end) break;
                next = end;

                while (*next != ' ' && *next != '\n' && *next != '\0') next++;

                // set the first index
                if (idxFirst == -1)
                    idxFirst = v;
                
                // set the previous index
                else if (idxPrevious == -1)
                    idxPrevious = v;
                
                // actually making triangles
                else {

                    // expand the indices if needed
                    if (numIndices + 3 >= indicesCap) {
                        indicesCap *= 2;
                        indices = realloc(indices, indicesCap * sizeof(int));
                    }

                    // store the indices
                    indices[numIndices++] = idxFirst - 1;
                    indices[numIndices++] = idxPrevious - 1;
                    indices[numIndices++] = v - 1;


                    // re-set the previous idx
                    idxPrevious = v;
                }
            }
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

    // close the file
    fclose(objFile);

    // return the mesh
    return m;
}

// destroy a mesh resource
void mesh_resource_destroy(mesh_resource *r) {
    free(r->vertices);
    free(r->indices);
    r->vertices = NULL;
    r->indices = NULL;
    r->numVertices = 0;
    r->numIndices = 0;
}