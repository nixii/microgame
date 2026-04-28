
#ifndef MG_RENDERER_H
#define MG_RENDERER_H

#include "microgame/util/color.h"

// renderer type
typedef struct renderer {
    int width;
    int height;
    color *pixels;
    float *depth_buffer;
} renderer;

// renderer functions
renderer renderer_new(int width, int height);
void renderer_clear(renderer *r);
void renderer_render_square(renderer *r, int x, int y, int w, int h, color c);
void renderer_render_triangle(renderer *r, int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, color c);
void renderer_destroy(renderer *r);

#endif // MG_RENDERER_H