
#include "microlib.h"
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int mouse_disabled = 0;

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
ms_data ml_get_mouse_delta() {
    return MS_DATA(get_mouse_delta());
}
ms_data ml_is_key_down(ms_data a) {
    assert(a.type == MS_DT_STRING);
    char c = a.value.str[0];
    size_t len = strlen(a.value.str);

    // switch based on the char
    switch (c) {
        case 'a':
            return MS_DATA(key_down(M_KEY_A));
        case 'b':
            return MS_DATA(key_down(M_KEY_B));
        case 'c':
            return MS_DATA(key_down(M_KEY_C));
        case 'd':
            return MS_DATA(key_down(M_KEY_D));
        case 'e':
            return MS_DATA(key_down(M_KEY_E));
        case 'f':
            return MS_DATA(key_down(M_KEY_F));
        case 'g':
            return MS_DATA(key_down(M_KEY_G));
        case 'h':
            return MS_DATA(key_down(M_KEY_H));
        case 'i':
            return MS_DATA(key_down(M_KEY_I));
        case 'j':
            return MS_DATA(key_down(M_KEY_J));
        case 'k':
            return MS_DATA(key_down(M_KEY_K));
        case 'l':
            return MS_DATA(key_down(M_KEY_L));
        case 'm':
            return MS_DATA(key_down(M_KEY_M));
        case 'n':
            return MS_DATA(key_down(M_KEY_N));
        case 'o':
            return MS_DATA(key_down(M_KEY_O));
        case 'p':
            return MS_DATA(key_down(M_KEY_P));
        case 'q':
            return MS_DATA(key_down(M_KEY_Q));
        case 'r':
            return MS_DATA(key_down(M_KEY_R));
        case 's':
            return MS_DATA(key_down(M_KEY_S));
        case 't':
            return MS_DATA(key_down(M_KEY_T));
        case 'u':
            return MS_DATA(key_down(M_KEY_U));
        case 'v':
            return MS_DATA(key_down(M_KEY_V));
        case 'w':
            return MS_DATA(key_down(M_KEY_W));
        case 'x':
            return MS_DATA(key_down(M_KEY_X));
        case 'y':
            return MS_DATA(key_down(M_KEY_Y));
        case 'z':
            return MS_DATA(key_down(M_KEY_Z));
        case 'S':
            return MS_DATA(key_down(M_KEY_SPACE));
        case 'L':
            if (len >= 2) {
                if (a.value.str[1] == 'S')
                    return MS_DATA(key_down(M_KEY_LSHIFT));
                if (a.value.str[1] == 'C')
                    return MS_DATA(key_down(M_KEY_LCTRL));
                if (a.value.str[1] == 'A')
                    return MS_DATA(key_down(M_KEY_LALT));
                if (a.value.str[1] == 'M')
                    return MS_DATA(key_down(M_KEY_LMETA));
            }
            fprintf(stderr, "%s is not a valid key.\n", a.value.str);
            exit(1);
        case 'R':
            if (len >= 2) {
                if (a.value.str[1] == 'S')
                    return MS_DATA(key_down(M_KEY_RSHIFT));
                if (a.value.str[1] == 'C')
                    return MS_DATA(key_down(M_KEY_RCTRL));
                if (a.value.str[1] == 'A')
                    return MS_DATA(key_down(M_KEY_RALT));
                if (a.value.str[1] == 'M')
                    return MS_DATA(key_down(M_KEY_RMETA));
            }
            fprintf(stderr, "%s is not a valid key.\n", a.value.str);
            exit(1);
        default:
            fprintf(stderr, "%s is not a valid key.\n", a.value.str);
            exit(1);
    }
}
ms_data ml_disable_mouse() {
    disable_mouse();
    mouse_disabled = 1;
    return (ms_data){ .type = MS_DT_NIL };
}
ms_data ml_enable_mouse() {
    enable_mouse();
    mouse_disabled = 0;
    return (ms_data){ .type = MS_DT_NIL };
}
ms_data ml_is_mouse_enabled() {
    return MS_DATA(mouse_disabled == 0);
}