

#include "velocity.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

// set a property of a velocity
void ms_interpreter_component_velocity_set_property(velocity *v, const char *propName, ms_data value) {

    // set the size of the velocity
    if (strcmp(propName, "velocity") == 0) {
        assert(value.type == MS_DT_VEC3 && !value.ptr);
        v->velocity = value.value.v3;
    }

    // not a valid property
    else {
        fprintf(stderr, "cannot set property %s on velocity\n", propName);
        exit(1);
    }
}

// get the property of a velocity
ms_data ms_interpreter_component_velocity_get_property(velocity *v, const char *propName) {

    // set the size of a velocity
    if (strcmp(propName, "velocity") == 0) {
        return (ms_data){ .type = MS_DT_VEC3, .ptr = FALSE, .value = (ms_data_value){ .v3 = v->velocity } };
    }

    // not a valid property
    else {
        fprintf(stderr, "cannot set property %s on velocity\n", propName);
        exit(1);
    }
}