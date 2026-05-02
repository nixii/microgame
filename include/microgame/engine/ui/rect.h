
#ifndef MG_UI_RECT_H
#define MG_UI_RECT_H

#include "microgame/renderer/renderer.h"
#include "microgame/util/color.h"

// a ui_rect type
typedef struct ui_rect {
    color c;
} ui_rect;

/*
 * ui_rect functions
*/

// allocate a ui_rect
ui_rect *ui_rect_new(color c);

// destroy a ui_rect
void ui_rect_destroy(ui_rect *r);

// render a ui_rect
void ui_rect_render(
    ui_rect *rect, 
    renderer *r, 
    int x, 
    int y, 
    int w, 
    int h, 
    color overlay);

#endif // MG_UI_RECT_H