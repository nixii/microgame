
#include "microgame/renderer/renderer.h"
#include "microgame/util/color.h"
#include <stdlib.h>

// create a new renderer
renderer renderer_new(int width, int height) {
    color *pixels = malloc(width * height * sizeof(color));
    return (renderer){
        .width = width,
        .height = height,
        .pixels = pixels
    };
}

// clear the renderer
void renderer_clear(renderer *r) {
    for (int y = 0; y < r->height; y++) {
        for (int x = 0; x < r->width; x++) {
            r->pixels[x + y * r->width] = rgb(0, 0, 0);
        }
    }
}

// render a square
void renderer_render_square(renderer *r, int x, int y, int w, int h, color c) {

    // blit
    for (int yy = y; yy < y + h; yy++) {
        for (int xx = x; xx <= x + w; xx++) {
            r->pixels[xx + yy * r->width] = c;
        }
    }
}

// destroy the renderer
void renderer_destroy(renderer *r) {
    free(r->pixels);
    r->pixels = NULL;
    r->width = 0;
    r->height = 0;
}