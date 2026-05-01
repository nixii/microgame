
#include "microgame/engine/ui/image.h"
#include <stdio.h>

// create a new image resource
ui_image *ui_image_new(image_resource *r) {
    ui_image *i = malloc(sizeof(ui_image));
    i->image = r;
    return i;
}

// destroy a ui image
void ui_image_destroy(ui_image *i) {
    free(i);
}

// TODO: scale the image
// render a ui image
void ui_image_render(ui_image *i, renderer *r, int x, int y, int w, int h) {
    int trueWidth = i->image->width > w ? w : i->image->width;
    int trueHeight = i->image->height > h ? h : i->image->height;

    renderer_render_image(r, x, y, trueWidth, trueHeight, 0, 0, i->image->pixels);
}