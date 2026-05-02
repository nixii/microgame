
#include "microgame/engine/ui/rect.h"

// allocate a ui_rect
ui_rect *ui_rect_new(color c) {
    ui_rect *r = malloc(sizeof(ui_rect));
    r->c = c;
    return r;
}

// destroy a ui_rect
void ui_rect_destroy(ui_rect *r) {
    free(r);
}

// render a ui_rect
void ui_rect_render(ui_rect *rect, renderer *r, int x, int y, int w, int h, color overlay) {
    renderer_render_rectangle(r, x, y, w, h, rect->c);
}