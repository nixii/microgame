
#ifndef MG_UIVEC_H
#define MG_UIVEC_H

typedef struct ui_vec {
    int scaleX, scaleY;
    int pixelsX, pixelsY;
} ui_vec;

/*
 * ui_vec functions
*/

// create a new empty ui_vec
ui_vec ui_vec_empty();

#endif // MG_UIVEC_H