
#include "microgame/renderer/renderer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// create a new renderer
renderer renderer_new(int width, int height) {
    color *pixels = malloc(width * height * sizeof(color));
    float *depthBuffer = malloc(width * height * sizeof(float));
    return (renderer){
        .width = width,
        .height = height,
        .pixels = pixels,
        .depthBuffer = depthBuffer
    };
}

// clear the renderer
void renderer_clear(renderer *r) {
    for (int y = 0; y < r->height; y++) {
        for (int x = 0; x < r->width; x++) {
            r->pixels[x + y * r->width] = rgb(0, 0, 0);
            r->depthBuffer[x + y * r->width] = 1e30;
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
static inline int edge(int ax, int ay, int bx, int by, int px, int py) {
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
    int area = edge(x1, y1, x2, y2, x3, y3);
    float invArea = 1.0f / area;
    int sign = area > 0 ? 1 : -1;

    // edge deltas
    int dW1dx = (y3 - y2);
    int dW1dy = -(x3 - x2);

    int dW2dx = (y1 - y3);
    int dW2dy = -(x1 - x3);

    int dW3dx = (y2 - y1);
    int dW3dy = -(x2 - x1);

    // initial edge values
    int w1_row = edge(x2, y2, x3, y3, minX, minY);
    int w2_row = edge(x3, y3, x1, y1, minX, minY);
    int w3_row = edge(x1, y1, x2, y2, minX, minY);

    // for every point
    for (int y = minY; y <= maxY; y++) {

        int w1 = w1_row;
        int w2 = w2_row;
        int w3 = w3_row;

        int idx = y * r->width + minX;

        for (int x = minX; x <= maxX; x++) {

            if (w1*sign >= 0 && w2*sign >= 0 && w3*sign >= 0) {

                w1 *= invArea;
                w2 *= invArea;
                w3 *= invArea;

                float z = w1*z1 + w2*z2 + w3*z3;


                if (z < r->depthBuffer[idx]) {
                    r->depthBuffer[idx] = z;
                    r->pixels[idx] = c;
                }
            }

            w1 += dW1dx;
            w2 += dW2dx;
            w3 += dW3dx;

            idx++;
        }

        w1_row += dW1dy;
        w2_row += dW2dy;
        w3_row += dW3dy;
    }
}

// destroy the renderer
void renderer_destroy(renderer *r) {
    free(r->pixels);
    r->pixels = NULL;
    r->width = 0;
    r->height = 0;
}