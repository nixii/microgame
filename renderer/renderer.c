
#include "microrender/renderer.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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

// render a triangle
void mr_renderer_render_triangle(mr_renderer *r, int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color)
{

    // calculate the bounds (super ugly)
    int minX = x1 < x2 ? (x1 < x3 ? x1 : x3) : (x2 < x3 ? x2 : x3);
    int maxX = x1 > x2 ? (x1 > x3 ? x1 : x3) : (x2 > x3 ? x2 : x3);
    int minY = y1 < y2 ? (y1 < y3 ? y1 : y3) : (y2 < y3 ? y2 : y3);
    int maxY = y1 > y2 ? (y1 > y3 ? y1 : y3) : (y2 > y3 ? y2 : y3);

    // the are of the triangle
    float area = fabs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);

    // for every point
    for (int y = minY; y <= maxY; y++)
    {
        for (int x = minX; x <= maxX; x++)
        {

            // the other triangle areas
            float a1 = fabs((x * (y2 - y3) + x2 * (y3 - y) + x3 * (y - y2)) / 2.0);
            float a2 = fabs((x1 * (y - y3) + x * (y3 - y1) + x3 * (y1 - y)) / 2.0);
            float a3 = fabs((x1 * (y2 - y) + x2 * (y - y1) + x * (y1 - y2)) / 2.0);

            // area add
            if (fabs(area - a1 - a2 - a3) < 0.5)
                r->pixels[x + y * r->width] = color;
        }
    }
}

// destroy a renderer
void mr_renderer_destroy(mr_renderer *r)
{
    free(r->pixels);
    r->pixels = NULL;
}