
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
    renderer_render_image_stretch(r, x, y, w, h, i->image->width, i->image->height, i->image->pixels);
}