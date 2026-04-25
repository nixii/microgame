
#include "render.h"
#include "triangle.h"
#include "color.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// create a new renderer which will draw to the pixel buffer
// this does not handle raylib at all
renderer renderer_new(int width, int height)
{

    // allocate the pixel buffer
    uint32_t *pixels = malloc(sizeof(uint32_t) * width * height);

    // if the pixel buffer failed then return an empty renderer and set errno
    if (pixels == NULL)
    {
        errno = 1;
        return (renderer){
            .width = 0,
            .height = 0,
            .pixels = NULL
        };
    }

    // on success, return the renderer
    return (renderer){
        .width = width,
        .height = height,
        .pixels = pixels
    };
}

// render a square
void renderer_render_square(renderer *r, int x, int y, int w, int h, uint32_t color)
{
    for (int xx = x; xx < x + w; xx++)
    {
        for (int yy = y; yy < y + h; yy++)
        {
            r->pixels[xx + yy * r->width] = color;
        }
    }
}

// render a triangle via scanning
void renderer_render_triangle(renderer *r, int *triangle, uint32_t color)
{
    int maxy = triangle[1];
    int miny = triangle[1];
    int maxx = triangle[0];
    int minx = triangle[0];

    // load the bounds
    for (int i = 0; i < 6; i += 2)
    {
        if (triangle[i] > maxx)
        {
            maxx = triangle[i];
        }
        else if (triangle[i] < minx)
        {
            minx = triangle[i];
        }
        if (triangle[i+1] > maxy)
        {
            maxy = triangle[i+1];
        }
        else if (triangle[i+1] < miny)
        {
            miny = triangle[i+1];
        }
    }

    // checking
    if (maxx >= r->width) return;
    if (minx < 0) return;
    if (maxy >= r->height) return;
    if (miny < 0) return;

    // rename
    int x1 = triangle[0]; int x2 = triangle[2]; int x3 = triangle[4];
    int y1 = triangle[1]; int y2 = triangle[3]; int y3 = triangle[5];

    // base area
    int a = triangle_area(x1, y1, x2, y2, x3, y3);

    // iterate
    for (int y = miny; y < maxy; y++)
    {
        for (int x = minx; x < maxx; x++)
        {

            // calculate the other areas
            int a1 = triangle_area(x, y, x2, y2, x3, y3);
            int a2 = triangle_area(x1, y1, x, y, x3, y3);
            int a3 = triangle_area(x1, y1, x2, y2, x, y);

            // check
            if ((a1 >= 0 && a2 >= 0 && a3 >= 0) ||
                (a1 <= 0 && a2 <= 0 && a3 <= 0))
            {
                r->pixels[x + y * r->width] = color;
            }
        }
    }
}

// clear the screen to black
void renderer_clear(renderer *r)
{
    renderer_clear_color(r, rgba(0, 0, 0, 255));
}

// clear the screen to a color
void renderer_clear_color(renderer *r, uint32_t color)
{
    for (int y = 0; y < r->height; y++)
    {
        for (int x = 0; x < r->width; x++)
        {
            r->pixels[y * r->width + x] = color;
        }
    }
}