
#include "vec3.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

void ms_interpreter_vec3_set_property(vec3 *v, const char *propName, ms_data value) {

    // vec3s only allow nums
    assert(value.type == MS_DT_NUMBER);

    // length (for more efficient setting and stuff)
    size_t len = strlen(propName);
    
    // check the chars
    if (len == 1) {
        switch (propName[0]) {
            case 'x':
                v->x = value.value.num;
                return;
            case 'y':
                v->y = value.value.num;
                return;
            case 'z':
                v->z = value.value.num;
                return;
            default:
                break;
        }
    }

    // fail
    fprintf(stderr, "ms_interpreter_vec3_set_property: %s is not a property\n", propName);
    exit(1);
}

// get a property on a vec3
ms_data ms_interpreter_vec3_get_property(vec3 *v, const char *propName) {

    // length (for more efficient setting and stuff)
    size_t len = strlen(propName);
    
    // check the chars
    if (len == 1) {
        switch (propName[0]) {
            case 'x':
                return (ms_data){ .type = MS_DT_NUMBER, .ptr = FALSE, .value = (ms_data_value){ .num = v->x } };
            case 'y':
                return (ms_data){ .type = MS_DT_NUMBER, .ptr = FALSE, .value = (ms_data_value){ .num = v->y } };
            case 'z':
                return (ms_data){ .type = MS_DT_NUMBER, .ptr = FALSE, .value = (ms_data_value){ .num = v->z } };
            default:
                break;
        }
    }

    // fail
    fprintf(stderr, "ms_interpreter_vec3_get_property: %s is not a property\n", propName);
    exit(1);
}