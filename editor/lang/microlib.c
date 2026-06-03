
#include "microlib.h"
#include <assert.h>
#include <stdio.h>
#include <math.h>

ms_data ml_cos(ms_data a) {
    assert(a.type == MS_DT_NUMBER);
    return MS_DATA(cosf(a.value.num));
}
ms_data ml_sin(ms_data a) {
    assert(a.type == MS_DT_NUMBER);
    return MS_DATA(sinf(a.value.num));
}
ms_data ml_sqrt(ms_data a) {
    assert(a.type == MS_DT_NUMBER);
    return MS_DATA(sqrtf(a.value.num));
}
ms_data ml_square(ms_data a) {
    assert(a.type == MS_DT_NUMBER);
    return MS_DATA(a.value.num * a.value.num);
}
ms_data ml_length(ms_data a) {
    switch (a.type) {
        case MS_DT_VEC2:
            return MS_DATA(sqrtf(
                a.value.v2.x * a.value.v2.x +
                a.value.v2.y * a.value.v2.y));
        case MS_DT_VEC3:
            return MS_DATA(sqrtf(
                a.value.v3.x * a.value.v3.x +
                a.value.v3.y * a.value.v3.y +
                a.value.v3.z * a.value.v3.z));
        case MS_DT_VEC4:
            return MS_DATA(sqrtf(
                a.value.v4.scaleX * a.value.v4.scaleX +
                a.value.v4.pixelsX * a.value.v4.pixelsX +
                a.value.v4.scaleY * a.value.v4.scaleY +
                a.value.v4.pixelsY * a.value.v4.pixelsY));
        default:
            fprintf(stderr, "can't get length of data type %d.\n", a.type);
            exit(1);
    }
}