
#include "microrender/renderer.h"
#include <stdlib.h>
#include <stdio.h>

// create a new renderer instance
mr_renderer mr_renderer_new(int width, int height)
{
    uint32_t *pixels = malloc(width * height * sizeof(uint32_t));
    return (mr_renderer){
        .width = width,
        .height = height,
        .pixels = pixels
    };
}

// clear the renderer
void mr_renderer_clear(mr_renderer *r, uint32_t color)
{
    for (int i = 0; i < r->width * r->height; i++)
    {
        r->pixels[i] = color;
    }
}

// render a square
void mr_renderer_render_square(mr_renderer *r, int x, int y, int w, int h, uint32_t color)
{
    int x2 = x + w;
    int y2 = y + h;

    // clip
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x2 > r->width)  x2 = r->width;
    if (y2 > r->height) y2 = r->height;

    // recompute size
    w = x2 - x;
    h = y2 - y;

    // skip if too small
    if (w <= 0 || h <= 0) return;

    // render
    for (int yy = y; yy < y + h; yy++)
    {
        for (int xx = x; xx < x + w; xx++)
        {
    
            r->pixels[xx + yy * r->width] = color;
        }
    }
}

// destroy a renderer
void mr_renderer_destroy(mr_renderer *r)
{
    free(r->pixels);
    r->pixels = NULL;
}