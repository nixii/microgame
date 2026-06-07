
#include "collider.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

// set a property of a collider
void ms_interpreter_component_collider_set_property(collider *c, const char *propName, ms_data value) {

    // set the size of the collider
    if (strcmp(propName, "size") == 0) {
        assert(value.type == MS_DT_VEC3);
        c->size = value.ptr ? *value.value.v3Ptr : value.value.v3;
    }

    // not a valid property
    else {
        fprintf(stderr, "cannot set property %s on collider\n", propName);
        exit(1);
    }
}

// get the property of a collider
ms_data ms_interpreter_component_collider_get_property(collider *c, const char *propName) {

    // set the size of a collider
    if (strcmp(propName, "size") == 0) {
        return (ms_data){ .type = MS_DT_VEC3, .ptr = FALSE, .value = (ms_data_value){ .v3 = c->size } };
    }

    // not a valid property
    else {
        fprintf(stderr, "cannot set property %s on collider\n", propName);
        exit(1);
    }
}