
#ifndef MICRORENDER_H
#define MICRORENDER_H

#include <stdint.h>

typedef struct mr_renderer {
    int width;
    int height;
    uint32_t *pixels;
} mr_renderer;

// create a new renderer instance
mr_renderer mr_renderer_new(int width, int height);

// clear the renderer
void mr_renderer_clear(mr_renderer *r, uint32_t color);

// render a square
void mr_renderer_render_square(mr_renderer *r, int x, int y, int w, int h, uint32_t color);

// destroy a renderer
void mr_renderer_destroy(mr_renderer *r);

// rgb
inline static uint32_t mr_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return (r) | (g << 8) | (b << 16) | (0xFF << 24);
}

// strip automatically
#ifndef MICRORENDER_NOSTROP
    #define renderer mr_renderer
    #define renderer_new mr_renderer_new
    #define renderer_clear mr_renderer_clear
    #define renderer_destroy mr_renderer_destroy
#endif

#endif // MICRORENDER_H