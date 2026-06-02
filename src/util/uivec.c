
#include "microgame/util/uivec.h"

// create an empty ui_vec
ui_vec ui_vec_empty() {
    return (ui_vec){
        .scaleX = 0, .scaleY = 0,
        .pixelsX = 0, .pixelsY = 0
    };
}

// ui vec with sX, pX, sY, pY
ui_vec ui_vec_new(float sX, float pX, float sY, float pY) {
    return (ui_vec){
        .scaleX = sX, .pixelsX = pX,
        .scaleY = sY, .pixelsY = pY
    };
}

// ops
ui_vec ui_vec_add(ui_vec a, ui_vec b) {
    return ui_vec_new(a.scaleX + b.scaleX, a.pixelsX + b.pixelsX, a.scaleY + b.scaleY, a.pixelsY + b.pixelsY);
}
ui_vec ui_vec_sub(ui_vec a, ui_vec b) {
    return ui_vec_new(a.scaleX - b.scaleX, a.pixelsX - b.pixelsX, a.scaleY - b.scaleY, a.pixelsY - b.pixelsY);
}
ui_vec ui_vec_mul(ui_vec a, float scalar) {
    return ui_vec_new(a.scaleX * scalar, a.pixelsX * scalar, a.scaleY * scalar, a.pixelsY * scalar);
}
ui_vec ui_vec_mul_components(ui_vec a, ui_vec b) {
    return ui_vec_new(a.scaleX * b.scaleX, a.pixelsX * b.pixelsX, a.scaleY * b.scaleY, a.pixelsY * b.pixelsY);
}
ui_vec ui_vec_div(ui_vec a, float scalar) {
    return ui_vec_new(a.scaleX / scalar, a.pixelsX / scalar, a.scaleY / scalar, a.pixelsY / scalar);
}