
#ifndef MG_UI_IMAGE_H
#define MG_UI_IMAGE_H

#include "microgame/engine/resource/image.h"
#include "microgame/renderer/renderer.h"

// the image type
typedef struct ui_image {
    image_resource *image;
} ui_image;

/*
 * ui image functions
*/

// create a new image resource
ui_image *ui_image_new(image_resource *r);

// destroy a ui image
void ui_image_destroy(ui_image *i);

// render a ui image
void ui_image_render(ui_image *i, renderer *r, int x, int y, int w, int h);

#endif // MG_UI_IMAGE_H