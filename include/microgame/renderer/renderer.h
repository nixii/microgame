
#ifndef MG_RENDERER_H
#define MG_RENDERER_H

#include "microgame/util/color.h"

// renderer type
typedef struct renderer {
    int width;
    int height;
    color *pixels;
    float *depthBuffer;
} renderer;

/*
 * renderer functions
*/

// create a brand-new renderer
renderer renderer_new(int width, int height);

// clear the whole screen
void renderer_clear(renderer *r);

// render a simple rectangle of color
void renderer_render_rectangle(renderer *r, int x, int y, int w, int h, color c);

// render a simple triangle of color
void renderer_render_triangle(renderer *r, int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, color c);

// destroy the renderer instance
void renderer_destroy(renderer *r);

// render an image as a rectangle to the screen
void renderer_render_image(renderer *r, int x, int y, int w, int h, int imgX, int imgY, color *pixels);

// render part of an image to the screen (a slice)
void renderer_render_image_slice(
    renderer *r, 
    int xx, 
    int yy, 
    int w,
    int h,
    int imgX, 
    int imgY, 
    int imgW,
    color *pixels,
    color overlay);

// draw an image to fill a rectangle (dumb sampling)
void renderer_render_image_stretch(
    renderer *r, 
    int x, int y, 
    int w, int h, 
    int imgW, int imgH,
    color *pixels);

#endif // MG_RENDERER_H