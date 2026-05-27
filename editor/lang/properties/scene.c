
#include "scene.h"
#include <stdio.h>
#include <string.h>

void ms_interpreter_scene_set_property(scene *s, const char *propName, ms_data value) {

    // only light direction so far
    if (strcmp(propName, "light_direction") == 0) {
        s->lightDir = value.value.v3;
    }

    // fail on all other results
    else {
        fprintf(stderr, "ms_interpreter_scene_set_property: scene has no property %s\n", propName);
        exit(1);
    }
}

ms_data ms_interpreter_scene_get_property(scene *s, const char *propName) {

    // only light direction rn
    if (strcmp(propName, "light_direction") == 0) {
        return (ms_data){ .type = MS_DT_VEC3, .value = (ms_data_value){ .v3 = s->lightDir } };
    }

    // fail on all other results
    else {
        fprintf(stderr, "ms_interpreter_scene_set_property: scene has no property %s\n", propName);
        exit(1);
    }
}