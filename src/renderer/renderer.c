
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
void renderer_render_rectangle(renderer *r, int x, int y, int w, int h, color c, color overlay) {

    // auto-overlay
    color mixed = rgb_overlay(c, overlay);

    // blit
    for (int yy = y; yy < y + h; yy++) {
        for (int xx = x; xx <= x + w; xx++) {

            // color mix
            color cc = rgb_overlay(r->pixels[xx + yy * r->width], mixed);

            // set the pixel
            r->pixels[xx + yy * r->width] = cc;
        }
    }
}

// triangle rendering helper
static inline long long edge(int ax, int ay, int bx, int by, int px, int py) {
    return (long long)(px - ax) * (by - ay) - (long long)(py - ay) * (bx - ax);
}

// render a triangle
void renderer_render_triangle(renderer *r, int x1, int y1, float z1, int x2, int y2, float z2, int x3, int y3, float z3, color c) {
    
    int minX = x1 < x2 ? (x1 < x3 ? x1 : x3) : (x2 < x3 ? x2 : x3);
    int maxX = x1 > x2 ? (x1 > x3 ? x1 : x3) : (x2 > x3 ? x2 : x3);
    int minY = y1 < y2 ? (y1 < y3 ? y1 : y3) : (y2 < y3 ? y2 : y3);
    int maxY = y1 > y2 ? (y1 > y3 ? y1 : y3) : (y2 > y3 ? y2 : y3);
    minX = minX < 0 ? 0 : (minX >= r->width ? r->width - 1 : minX);
    maxX = maxX < 0 ? 0 : (maxX >= r->width ? r->width - 1 : maxX);
    minY = minY < 0 ? 0 : (minY >= r->height ? r->height - 1 : minY);
    maxY = maxY < 0 ? 0 : (maxY >= r->height ? r->height - 1 : maxY);

    long long area = edge(x1, y1, x2, y2, x3, y3);
    if (area == 0) return;
    float invArea = fabsf(1.0f / (float)area);
    int sign = area > 0 ? 1 : -1;

    int dW1dx = (y3 - y2);
    int dW1dy = -(x3 - x2);
    int dW2dx = (y1 - y3);
    int dW2dy = -(x1 - x3);
    int dW3dx = (y2 - y1);
    int dW3dy = -(x2 - x1);

    long long w1_row = edge(x2, y2, x3, y3, minX, minY);
    long long w2_row = edge(x3, y3, x1, y1, minX, minY);
    long long w3_row = edge(x1, y1, x2, y2, minX, minY);

    z1 = 1.0f / z1;
    z2 = 1.0f / z2;
    z3 = 1.0f / z3;

    for (int y = minY; y <= maxY; y++) {

        long long w1 = w1_row;
        long long w2 = w2_row;
        long long w3 = w3_row;

        int idx = y * r->width + minX;

        for (int x = minX; x <= maxX; x++) {

            if (w1*sign >= 0 && w2*sign >= 0 && w3*sign >= 0) {

                float fw1 = (float)w1;
                float fw2 = (float)w2;
                float fw3 = (float)w3;
                float z = (fw1*invArea*z1 + fw2*invArea*z2 + fw3*invArea*z3);

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

// render an image
void renderer_render_image(
    renderer *r, 
    int x, 
    int y, 
    int w, 
    int h, 
    int imgX, 
    int imgY, 
    color *pixels)
{

    // base start/end positions
    int startX = imgX;
    int startY = imgY;
    int endX = w - imgX;
    int endY = h - imgY;

    // clip positions
    if (x + startX < 0) startX = -x;
    if (y + startY < 0) startY = -y;
    if (x + endX > r->width) endX = r->width - x;
    if (y + endY > r->height) endY = r->height - y;

    // draw the pixels
    for (int yy = startY; yy < endY; yy++) {
        for (int xx = startX; xx < endX; xx++) {
            r->pixels[(y + yy) * r->width + x + xx] = pixels[yy * w + xx];
        }
    }
}

// render an image
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
    color overlay)
{

    // starting x positions
    int startX = xx, startY = yy;
    if (xx < 0) {
        startX = 0;
        imgX -= xx;
    }
    if (yy < 0) {
        startY = 0;
    }

    // calculate the end position on the screen
    int endX = xx + w;
    int endY = yy + h;

    // cutoff
    endX = endX > r->width ? r->width : endX;
    endY = endY > r->height ? r->height : endY;

    // the starting position on the image
    int imgXStart = imgX;

    // for every pixel
    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {

            // get the colors
            color src = pixels[imgY * imgW + imgX];

            // skip if invisible (since fonts have a lot of empty this is worth it)
            if (alpha(src) == 0) {
                imgX++; 
                continue;
            }

            // destination
            color dest = r->pixels[y * r->width + x];

            // mix the colors
            color mixed = rgb_composite(dest, rgb_overlay(src, overlay));

            // blit the color
            r->pixels[y * r->width + x] = mixed;
            imgX++;
        }

        // go to the next row of the image
        imgY++;
        imgX = imgXStart;
    }
}

// render an image and stretch it like uv
void renderer_render_image_stretch(
    renderer *r, 
    int x, int y, 
    int w, int h, 
    int imgW, int imgH,
    color *pixels,
    color overlay) 
{

    // clip the image
    int startX = x < 0 ? 0 : x;
    int startY = y < 0 ? 0 : y;
    int endX = (x + w > r->width) ? r->width : (x + w);
    int endY = (y + h > r->height) ? r->height : (y + h);

    // scale
    float xScale = (float)imgW / (float)w;
    float yScale = (float)imgH / (float)h;
    
    // render the pixels
    for (int yy = startY; yy < endY; yy++) {
        for (int xx = startX; xx < endX; xx++) {
            
            // get the source location
            int srcX = (int)((xx - x) * xScale);
            int srcY = (int)((yy - y) * yScale);

            // get the color
            color mixed = rgb_overlay(r->pixels[yy * r->width + xx], rgb_overlay(pixels[srcY * imgW + srcX], overlay));

            // draw
            r->pixels[yy * r->width + xx] = mixed;
        }
    }
}