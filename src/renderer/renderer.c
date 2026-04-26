
#include "microgame/renderer/renderer.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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

// render a triangle
void renderer_render_triangle(renderer *r, int x1, int y1, int x2, int y2, int x3, int y3, color c) {
    
    // calculate the bounds (super ugly)
    int minX = x1 < x2 ? (x1 < x3 ? x1 : x3) : (x2 < x3 ? x2 : x3);
    int maxX = x1 > x2 ? (x1 > x3 ? x1 : x3) : (x2 > x3 ? x2 : x3);
    int minY = y1 < y2 ? (y1 < y3 ? y1 : y3) : (y2 < y3 ? y2 : y3);
    int maxY = y1 > y2 ? (y1 > y3 ? y1 : y3) : (y2 > y3 ? y2 : y3);
    minX = minX < 0 ? 0 : (minX >= r->width ? r->width - 1 : minX);
    maxX = maxX < 0 ? 0 : (maxX >= r->width ? r->width - 1 : maxX);
    minY = minY < 0 ? 0 : (minY >= r->height ? r->height - 1 : minY);
    maxY = maxY < 0 ? 0 : (maxY >= r->height ? r->height - 1 : maxY);

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
                r->pixels[x + y * r->width] = c;
            
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