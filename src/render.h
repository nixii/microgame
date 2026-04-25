
#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>

typedef struct renderer {
    int width;
    int height;
    uint32_t *pixels;
} renderer;

renderer renderer_new(int width, int height);

void renderer_clear_color(renderer *r, uint32_t color);
void renderer_clear(renderer *r);

void renderer_render_square(renderer *r, int x, int y, int w, int h, uint32_t color);
void renderer_render_triangle(renderer *r, int *triangle, uint32_t color);

#endif // RENDER_H