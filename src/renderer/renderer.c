
#include "microgame/renderer/renderer.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// create a new renderer
renderer renderer_new(int width, int height) {
    color *pixels = malloc(width * height * sizeof(color));
    float *depth_buffer = malloc(width * height * sizeof(float));
    return (renderer){
        .width = width,
        .height = height,
        .pixels = pixels,
        .depth_buffer = depth_buffer
    };
}

// clear the renderer
void renderer_clear(renderer *r) {
    for (int y = 0; y < r->height; y++) {
        for (int x = 0; x < r->width; x++) {
            r->pixels[x + y * r->width] = rgb(0, 0, 0);
            r->depth_buffer[x + y * r->width] = 1e30f; // some large number ¯\_(ツ)_/¯
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

// triangle rendering helper
static inline float edge(int ax, int ay, int bx, int by, int px, int py) {
    return (px - ax) * (by - ay) - (py - ay) * (bx - ax);
}

// render a triangle
void renderer_render_triangle(renderer *r, int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, color c) {
    
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
    float area = edge(x1, y1, x2, y2, x3, y3);

    // for every point
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            float w1 = edge(x2,y2,x3,y3,x,y);
            float w2 = edge(x3,y3,x1,y1,x,y);
            float w3 = edge(x1,y1,x2,y2,x,y);

            if ((w1 >= 0 && w2 >= 0 && w3 >= 0) ||
                (w1 <= 0 && w2 <= 0 && w3 <= 0)) {
                float invArea = 1.0f / area;

                w1 *= invArea;
                w2 *= invArea;
                w3 *= invArea;

                float z = w1*z1 + w2*z2 + w3*z3;

                int idx = x + y * r->width;

                if (z < r->depth_buffer[idx]) {
                    r->depth_buffer[idx] = z;
                    r->pixels[idx] = c;
                }
            }
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