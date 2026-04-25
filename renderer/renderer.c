
#include "microrender/renderer.h"
#include <stdlib.h>

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

// destroy a renderer
void mr_renderer_destroy(mr_renderer *r)
{
    free(r->pixels);
    r->pixels = NULL;
}