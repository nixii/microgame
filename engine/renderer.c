
#include <errno.h>
#include <stdlib.h>
#include "microengine/renderer.h"

/*
 * create a new renderer with given width and height
 * allocates the pixels 
*/
me_renderer me_renderer_new(int width, int height)
{

    // allocate the pixels
    uint32_t *pixels = malloc(width * height * sizeof(uint32_t));

    // skip out if malloc failed
    if (pixels == NULL)
    {
        return (me_renderer){
            .width = 0,
            .height = 0,
            .pixels = NULL
        };
    }

    // on success, return the new renderer instance
    return (me_renderer){
        .width = width,
        .height = height,
        .pixels = pixels
    };
}

/*
 * clear the entire screen with a color
*/
void me_renderer_clear(me_renderer *r, uint32_t color)
{

    // for each pixel, set its color
    for (int y = 0; y < r->height; y++)
    {
        for (int x = 0; x < r->width; x++)
        {
            r->pixels[x + y * r->width] = color;
        }
    }
}

/*
 * render a simple square onto the renderer
*/
void me_renderer_render_square(me_renderer *r, int x, int y, int w, int h, uint32_t color)
{

    // bounds check
    if (x < 0 || y < 0 || x+w >= r->width || y+h >= r->height)
        return;

    // simple iteration
    for (int yy = y; yy <= y + h; yy++)
    {
        for (int xx = x; xx <= x + w; xx++)
        {
            r->pixels[xx + yy * r->width] = color;
        }
    }
}

/*
 * free the renderer, do not use the renderer after calling thiss
*/
void me_renderer_destroy(me_renderer *r)
{

    // free the pixels
    free(r->pixels);

    // set all the values to null
    r->width = 0;
    r->height = 0;
    r->pixels = NULL;
}