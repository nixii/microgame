
#include "microgame/util/color.h"

color rgb_overlay(color base, color overlay) {
    
    // split up the base
    float br = red(base) / 255.0f;
    float bg = green(base) / 255.0f;
    float bb = blue(base) / 255.0f;

    // split up the overlay
    float or = red(overlay) / 255.0f;
    float og = green(overlay) / 255.0f;
    float ob = blue(overlay) / 255.0f;
    float oa = alpha(overlay) / 255.0f;

    // combine them
    return rgba(
        255 * (br * (1 - oa) + or * oa),
        255 * (bg * (1 - oa) + og * oa),
        255 * (bb * (1 - oa) + ob * oa),
        alpha(base)
    );
}

color rgb_composite(color dest, color src)
{
    
    float sr = red(src) / 255.0f;
    float sg = green(src) / 255.0f;
    float sb = blue(src) / 255.0f;
    float sa = alpha(src) / 255.0f;

    float dr = red(dest) / 255.0f;
    float dg = green(dest) / 255.0f;
    float db = blue(dest) / 255.0f;

    float r = sr * sa + dr * (1.0f - sa);
    float g = sg * sa + dg * (1.0f - sa);
    float b = sb * sa + db * (1.0f - sa);

    return rgb(
        (int)(r * 255.0f),
        (int)(g * 255.0f),
        (int)(b * 255.0f)
    );
}