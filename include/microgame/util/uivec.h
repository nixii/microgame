
#ifndef MG_UIVEC_H
#define MG_UIVEC_H

typedef struct ui_vec {
    float scaleX, scaleY;
    float pixelsX, pixelsY;
} ui_vec;

/*
 * ui_vec functions
*/

// create a new empty ui_vec
ui_vec ui_vec_empty();

// ui vec with sX, pX, sY, pY
ui_vec ui_vec_new(float sX, float pX, float sY, float pY);

#endif // MG_UIVEC_H