
#include "microgame/util/uivec.h"

// create an empty ui_vec
ui_vec ui_vec_empty() {
    return (ui_vec){
        .scaleX = 0, scaleY = 0,
        .pixelsX = 0, .pixelsY = 0
    };
}