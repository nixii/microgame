
#include "microgame/util/uivec.h"

// create an empty ui_vec
ui_vec ui_vec_empty() {
    return (ui_vec){
        .scaleX = 0, .scaleY = 0,
        .pixelsX = 0, .pixelsY = 0
    };
}

// ui vec with sX, pX, sY, pY
ui_vec ui_vec_new(float sX, int pX, float sY, int pY) {
    return (ui_vec){
        .scaleX = sX, .pixelsX = pX,
        .scaleY = sY, .pixelsY = pY
    };
}