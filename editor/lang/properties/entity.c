
#include "entity.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

static float TO_DEG_SCALAR = 180.0 / PI;
static float TO_RAD_SCALAR = PI / 180;

void ms_interpreter_entity_set_property(scene *s, entity e, const char *propName, ms_data value) {

    if (strcmp(propName, "position") == 0) {
        assert(value.type == MS_DT_VEC3 && !value.ptr);
        get_transform(s, e)->pos = value.value.v3;
    }

    else if (strcmp(propName, "rotation") == 0) {
        assert(value.type == MS_DT_VEC3 && !value.ptr);
        get_transform(s, e)->rot = MUL(value.value.v3, TO_RAD_SCALAR);
    }

    else if (strcmp(propName, "scale") == 0) {
        assert(value.type == MS_DT_VEC3 && !value.ptr);
        get_transform(s, e)->scale = value.value.v3;
    }

    else {
        fprintf(stderr, "failed to set property \"%s\" of entity.\n", propName);
        exit(1);
    }
}

ms_data ms_interpreter_entity_get_property(scene *s, entity e, const char *propName) {

    if (strcmp(propName, "position") == 0) {
        vec3 pos = get_transform(s, e)->pos;
        return (ms_data){ .type = MS_DT_VEC3, .ptr = FALSE, .value = (ms_data_value){ .v3 = pos } };
    }

    else if (strcmp(propName, "rotation") == 0) {
        vec3 rot = get_transform(s, e)->rot;
        return (ms_data){ .type = MS_DT_VEC3, .ptr = FALSE, .value = (ms_data_value){ .v3 = MUL(rot, TO_DEG_SCALAR) } };
    }

    else if (strcmp(propName, "scale") == 0) {
        return (ms_data){ .type = MS_DT_VEC3, .ptr = FALSE, .value = (ms_data_value){ .v3 = get_transform(s, e)->scale } };
    }

    else {
        fprintf(stderr, "failed to set property \"%s\" of entity.\n", propName);
        exit(1);
    }

}

ms_data ms_interpreter_entity_attach_component(scene *s, entity e, ms_data value) {

    // depending on the component type
    switch (value.type) {

        // attach a collider component
        case MS_DT_COMPONENT_COLLIDER:
            if (value.ptr) attach_collider(s, e, *value.value.colliderPtr);
            else attach_collider(s, e, value.value.collider);
            return (ms_data){ .type = MS_DT_COMPONENT_COLLIDER, .ptr = TRUE, .value = { .colliderPtr = get_collider(s, e) } };

        // velocities
        case MS_DT_COMPONENT_VELOCITY:
            if (value.ptr) attach_velocity(s, e, *value.value.velocityPtr);
            else attach_velocity(s, e, value.value.velocity);
            return (ms_data){ .type = MS_DT_COMPONENT_VELOCITY, .ptr = TRUE, .value = { .velocityPtr = get_velocity(s, e) } };
        
        // this component is not supported
        default:
            fprintf(stderr, "no way to attach %d\n", value.type);
            exit(1);
    }
}