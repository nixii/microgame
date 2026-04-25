
#ifndef MICROENGINE_RENDERER_H
#define MICROENGINE_RENDERER_H

#include <stdint.h>

/*
 * the most basic renderer structure
 * simply will render onto this pixelbuffer
*/
typedef struct me_renderer {
    int width;
    int height;
    uint32_t *pixels;
} me_renderer;

/*
 * create a new renderer with given width and height
 * allocates the pixels 
*/
me_renderer me_renderer_new(int width, int height);

/*
 * clear the entire screen with a color
*/
void me_renderer_clear(me_renderer *r, uint32_t color);

/*
 * render a simple square onto the renderer
*/
void me_renderer_render_square(me_renderer *r, int x, int y, int w, int h, uint32_t color);

/*
 * free the renderer, do not use the renderer after calling thiss
*/
void me_renderer_destroy(me_renderer *r);

#endif // MICROENGINE_RENDERER_H