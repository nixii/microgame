
#include "mesh.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

// set a property of a collimeshder
void ms_interpreter_component_mesh_set_property(mesh *m, const char *propName, ms_data value) {

    // set the size of the mesh
    if (strcmp(propName, "color") == 0) {
        assert(value.type == MS_DT_VEC3 && !value.ptr);
        m->color = rgb(value.value.v3.x, value.value.v3.y, value.value.v3.z);
    }

    // not a valid property
    else {
        fprintf(stderr, "cannot set property %s on mesh\n", propName);
        exit(1);
    }
}

// get the property of a mesh
ms_data ms_interpreter_component_mesh_get_property(mesh *m, const char *propName) {

    // set the size of a mesh
    if (strcmp(propName, "color") == 0) {
        return (ms_data){ .type = MS_DT_VEC3, .ptr = FALSE, .value = (ms_data_value){ .v3 = vec3_new(red(m->color), green(m->color), blue(m->color)) } };
    }

    // not a valid property
    else {
        fprintf(stderr, "cannot set property %s on mesh\n", propName);
        exit(1);
    }
}