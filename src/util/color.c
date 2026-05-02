
#include "microgame/util/color.h"

color rgb_overlay(color base, color overlay) {
    
    // split up the base
    float br = red(base) / 256.0f;
    float bg = green(base) / 256.0f;
    float bb = blue(base) / 256.0f;
    float ba = alpha(base) / 256.0f;

    // split up the overlay
    float or = red(overlay) / 256.0f;
    float og = green(overlay) / 256.0f;
    float ob = blue(overlay) / 256.0f;
    float oa = alpha(overlay) / 256.0f;

    // combine them
    return rgba(
        255 * (br * (1 - oa) + or * oa),
        255 * (bg * (1 - oa) + og * oa),
        255 * (bb * (1 - oa) + ob * oa),
        255 * (ba)
    );
}