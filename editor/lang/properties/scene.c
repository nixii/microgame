
#include "scene.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

void ms_interpreter_scene_set_property(scene *s, const char *propName, ms_data value) {

    // only light direction so far
    if (strcmp(propName, "light_direction") == 0) {
        assert(value.type == MS_DT_VEC3);
        s->lightDir = value.ptr ? *value.value.v3Ptr : value.value.v3;
    }

    // camera stuff
    else if (strcmp(propName, "camera_position") == 0) {
        assert(value.type == MS_DT_VEC3);
        s->camera.transform.pos = value.ptr ? *value.value.v3Ptr : value.value.v3;
    }
    else if (strcmp(propName, "camera_rotation") == 0) {
        assert(value.type == MS_DT_VEC3);
        s->camera.transform.rot = value.ptr ? *value.value.v3Ptr : value.value.v3;
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
        return (ms_data){ .type = MS_DT_VEC3, .ptr = FALSE, .value = (ms_data_value){ .v3 = s->lightDir } };
    }

    // camera stuff
    else if (strcmp(propName, "camera_position") == 0) {
        return MS_DATA(s->camera.transform.pos);
    }
    else if (strcmp(propName, "camera_rotation") == 0) {
        return MS_DATA(s->camera.transform.rot);
    }

    // fail on all other results
    else {
        fprintf(stderr, "ms_interpreter_scene_set_property: scene has no property %s\n", propName);
        exit(1);
    }
}