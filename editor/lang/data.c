
#include "data.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

// fail
static ms_data fail(const char *msg) {
    fprintf(stderr, "%s", msg);
    exit(1);
}

ms_data ms_data_bool(int truthy) { return (ms_data){ .type = MS_DT_BOOL, .ptr = FALSE, .value = (ms_data_value){ .boolean = truthy } }; }
ms_data ms_data_number(float num) { return (ms_data){ .type = MS_DT_NUMBER, .ptr = FALSE, .value = (ms_data_value){ .num = num } }; }
ms_data ms_data_vec2(vec2 v2) { return (ms_data){ .type = MS_DT_VEC2, .ptr = FALSE, .value = (ms_data_value){ .v2 = v2 } }; }
ms_data ms_data_vec3(vec3 v3) { return (ms_data){ .type = MS_DT_VEC3, .ptr = FALSE, .value = (ms_data_value){ .v3 = v3 } }; }
ms_data ms_data_vec4(ui_vec v4) { return (ms_data){ .type = MS_DT_VEC4, .ptr = FALSE, .value = (ms_data_value){ .v4 = v4 } }; }

// put scalar second
static void normalize_scalar(ms_data *a, ms_data *b) {
    if (a->type == MS_DT_NUMBER && b->type != MS_DT_NUMBER) {
        ms_data temp = *a;
        *a = *b;
        *b = temp;
    }
}

// resolve pointers
static void resolve_pointers(ms_data *a, ms_data *b) {
    if (a->ptr) {
        a->ptr = FALSE;
        a->value = *((ms_data_value *)a->value.genPtr);
    }
    if (b->ptr) {
        b->ptr = FALSE;
        b->value = *((ms_data_value *)b->value.genPtr);
    }
}

// perform operations
ms_data ms_data_mul(ms_data a, ms_data b) {
    normalize_scalar(&a, &b);
    resolve_pointers(&a, &b);
    switch (a.type) {
        case MS_DT_NUMBER: return MS_DATA(a.value.num * b.value.num);
        case MS_DT_VEC2: return b.type == MS_DT_NUMBER ? MS_DATA(MUL(a.value.v2, b.value.num)) : MS_DATA(MUL_COMPONENTS(a.value.v2, b.value.v2));
        case MS_DT_VEC3: return b.type == MS_DT_NUMBER ? MS_DATA(MUL(a.value.v3, b.value.num)) : MS_DATA(MUL_COMPONENTS(a.value.v3, b.value.v3));
        case MS_DT_VEC4: return b.type == MS_DT_NUMBER ? MS_DATA(MUL(a.value.v4, b.value.num)) : MS_DATA(MUL_COMPONENTS(a.value.v4, b.value.v4));
        default: return fail("can't do this.\n");
    }
}
ms_data ms_data_div(ms_data a, ms_data b) {
    normalize_scalar(&a, &b);
    resolve_pointers(&a, &b);
    switch (a.type) {
        case MS_DT_NUMBER: return MS_DATA(a.value.num / b.value.num);
        case MS_DT_VEC2: return b.type == MS_DT_NUMBER ? MS_DATA(DIV(a.value.v2, b.value.num)) : fail("can't divide by a vec.\n");
        case MS_DT_VEC3: return b.type == MS_DT_NUMBER ? MS_DATA(DIV(a.value.v3, b.value.num)) : fail("can't divide by a vec.\n");
        case MS_DT_VEC4: return b.type == MS_DT_NUMBER ? MS_DATA(DIV(a.value.v4, b.value.num)) : fail("can't divide by a vec.\n");
        default: return fail("can't do this.\n");
    }
}

ms_data ms_data_add(ms_data a, ms_data b) {
    assert(a.type == b.type);
    resolve_pointers(&a, &b);
    switch (a.type) {
        case MS_DT_NUMBER: return MS_DATA(a.value.num + b.value.num);
        case MS_DT_VEC2:
            return MS_DATA(ADD(a.value.v2, b.value.v2));
        case MS_DT_VEC3:
            return MS_DATA(ADD(a.value.v3, b.value.v3));
        case MS_DT_VEC4:
            return MS_DATA(ADD(a.value.v4, b.value.v4));
        case MS_DT_BOOL: return MS_DATA(a.value.boolean && b.value.boolean);
        default: return fail("can't do this.\n");
    }
}
ms_data ms_data_sub(ms_data a, ms_data b) {
    assert(a.type == b.type);
    switch (a.type) {
        case MS_DT_NUMBER: return MS_DATA(a.value.num - b.value.num);
        case MS_DT_VEC2:
            return MS_DATA(ADD(a.value.v2, b.value.v2));
        case MS_DT_VEC3:
            return MS_DATA(ADD(a.value.v3, b.value.v3));
        case MS_DT_VEC4:
            return MS_DATA(ADD(a.value.v4, b.value.v4));
        default: return fail("can't do this.\n");
    }
}

ms_data ms_data_eq(ms_data a, ms_data b) {
    if (a.type != b.type) return MS_DATA(0);
    switch (a.type) {
        case MS_DT_STRING: return ms_data_bool(strcmp(a.value.str, b.value.str) == 0);
        case MS_DT_NUMBER: return ms_data_bool(a.value.num == b.value.num);
        case MS_DT_BOOL: return ms_data_bool(a.value.boolean == b.value.boolean);
        case MS_DT_NIL: return ms_data_bool(1);

        case MS_DT_VEC2: return ms_data_bool(a.value.v2.x == b.value.v2.x && a.value.v2.y == b.value.v2.y);
        case MS_DT_VEC3: return ms_data_bool(a.value.v3.x == b.value.v3.x && a.value.v3.y == b.value.v3.y && a.value.v3.z == b.value.v3.z);
        case MS_DT_VEC4: return ms_data_bool(a.value.v4.scaleX == b.value.v4.scaleX && a.value.v4.pixelsX == b.value.v4.pixelsX && a.value.v4.scaleY == b.value.v4.scaleY && a.value.v4.pixelsY == b.value.v4.pixelsY);

        case MS_DT_SCENE: return ms_data_bool(memcmp(&a.value.scene, &b.value.scene, sizeof(a.value.scene)) == 0);
        case MS_DT_ENTITY: return ms_data_bool(a.value.entity == b.value.entity);
        default: return ms_data_bool(0);
    }
}
ms_data ms_data_lt(ms_data a, ms_data b) {
    assert(a.type == b.type && a.type == MS_DT_NUMBER);
    return MS_DATA(a.value.num < b.value.num);
}
ms_data ms_data_gt(ms_data a, ms_data b) {
    assert(a.type == b.type && a.type == MS_DT_NUMBER);
    return MS_DATA(a.value.num > b.value.num);
}
ms_data ms_data_or(ms_data a, ms_data b) {
    assert(a.type == b.type && a.type == MS_DT_BOOL);
    return MS_DATA(a.value.boolean || b.value.boolean);
}