
#include "entity.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

void ms_interpreter_entity_set_property(scene *s, entity e, const char *propName, ms_data value) {

    if (strcmp(propName, "position") == 0) {
        assert(value.type == MS_DT_VEC3);
        get_transform(s, e)->pos = value.value.v3;
    }

    else if (strcmp(propName, "rotation") == 0) {
        assert(value.type == MS_DT_VEC3);
        vec3 oVal = value.value.v3;
        get_transform(s, e)->rot = vec3_new(oVal.x * 180.0 / PI, oVal.y * 180.0 / PI, oVal.z * 180.0 / PI);
    }

    else if (strcmp(propName, "scale") == 0) {
        assert(value.type == MS_DT_VEC3);
        get_transform(s, e)->scale = value.value.v3;
    }

    else {
        fprintf(stderr, "failed to set property \"%s\" of entity.\n", propName);
        exit(1);
    }
}