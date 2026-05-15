
#include "microgame/input.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

int key_to_rl(key k) {
    switch (k) {
        case M_KEY_A: return KEY_A;
        case M_KEY_B: return KEY_B;
        case M_KEY_C: return KEY_C;
        case M_KEY_D: return KEY_D;
        case M_KEY_E: return KEY_E;
        case M_KEY_F: return KEY_F;
        case M_KEY_G: return KEY_G;
        case M_KEY_H: return KEY_H;
        case M_KEY_I: return KEY_I;
        case M_KEY_J: return KEY_J;
        case M_KEY_K: return KEY_K;
        case M_KEY_L: return KEY_L;
        case M_KEY_M: return KEY_M;
        case M_KEY_N: return KEY_N;
        case M_KEY_O: return KEY_O;
        case M_KEY_P: return KEY_P;
        case M_KEY_Q: return KEY_Q;
        case M_KEY_R: return KEY_R;
        case M_KEY_S: return KEY_S;
        case M_KEY_T: return KEY_T;
        case M_KEY_U: return KEY_U;
        case M_KEY_V: return KEY_V;
        case M_KEY_W: return KEY_W;
        case M_KEY_X: return KEY_X;
        case M_KEY_Y: return KEY_Y;
        case M_KEY_Z: return KEY_Z;
        case M_KEY_0: return KEY_ZERO;
        case M_KEY_1: return KEY_ONE;
        case M_KEY_2: return KEY_TWO;
        case M_KEY_3: return KEY_THREE;
        case M_KEY_4: return KEY_FOUR;
        case M_KEY_5: return KEY_FIVE;
        case M_KEY_6: return KEY_SIX;
        case M_KEY_7: return KEY_SEVEN;
        case M_KEY_8: return KEY_EIGHT;
        case M_KEY_9: return KEY_NINE;
        case M_KEY_GRAVE: return KEY_GRAVE;
        case M_KEY_MINUS: return KEY_MINUS;
        case M_KEY_EQUALS: return KEY_EQUAL;
        case M_KEY_COMMA: return KEY_COMMA;
        case M_KEY_PERIOD: return KEY_PERIOD;
        case M_KEY_SLASH: return KEY_SLASH;
        case M_KEY_SEMICOLON: return KEY_SEMICOLON;
        case M_KEY_QUOTE: return KEY_APOSTROPHE;
        case M_KEY_LBRACKET: return KEY_LEFT_BRACKET;
        case M_KEY_RBRACKET: return KEY_RIGHT_BRACKET;
        case M_KEY_BACKSLASH: return KEY_BACKSLASH;
        case M_KEY_SPACE: return KEY_SPACE;
        case M_KEY_LSHIFT: return KEY_LEFT_SHIFT;
        case M_KEY_RSHIFT: return KEY_RIGHT_SHIFT;
        case M_KEY_ESC: return KEY_ESCAPE;
        case M_KEY_RETURN: return KEY_ENTER;
        case M_KEY_BACKSPACE: return KEY_BACKSPACE;
        case M_KEY_DELETE: return KEY_DELETE;
        case M_KEY_TAB: return KEY_TAB;
        case M_KEY_LCTRL: return KEY_LEFT_CONTROL;
        case M_KEY_RCTRL: return KEY_RIGHT_CONTROL;
        case M_KEY_LALT: return KEY_LEFT_ALT;
        case M_KEY_RALT: return KEY_RIGHT_ALT;
        case M_KEY_LMETA: return KEY_LEFT_SUPER;
        case M_KEY_RMETA: return KEY_RIGHT_SUPER;
        case M_KEY_LEFT: return KEY_LEFT;
        case M_KEY_RIGHT: return KEY_RIGHT;
        case M_KEY_UP: return KEY_UP;
        case M_KEY_DOWN: return KEY_DOWN;
        default: return 0;
    }
}

int mouse_to_rl(int mouse) {
    switch (mouse) {
        case 1: return MOUSE_BUTTON_LEFT;
        case 2: return MOUSE_BUTTON_RIGHT;
        case 3: return MOUSE_BUTTON_MIDDLE;
        default:
            fprintf(stderr, "no mouse button %d supported.\n", mouse);
            exit(1);
    }
}