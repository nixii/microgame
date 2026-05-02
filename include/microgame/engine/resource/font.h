
#ifndef MG_RESOURCE_FONT_H
#define MG_RESOURCE_FONT_H

#include "microgame/util/color.h"
#include "microgame/engine/resource/image.h"

// the font struct
typedef struct font_resource {
    int charWidth, charHeight;
    int horizontalSpacing, verticalSpacing;
    image_resource source;
} font_resource;

// for handling the bounds
typedef struct font_bounds {
    int x, y, w, h;
} font_bounds;

/*
 * font resource functions
*/

// create a new font resource
font_resource font_resource_from(
    const char *path, 
    int charWidth, 
    int charHeight, 
    int horizontalSpacing, 
    int verticalSpacing);

// destroy a font resource
void font_resource_destroy(font_resource *r);

// get the bounds of a letter
font_bounds font_resource_get_bounds(font_resource *r, char c);

// get the width of text in pixels
int font_resource_get_width(font_resource *r, const char *text);

// get the height of text in pixels
int font_resource_get_height(font_resource *r, const char *text);

// get the amount of characters that fit in a line of specified width
int font_resource_chars_per_line(font_resource *r, int width);

// get the amount of columns that fit in a line of specified height
int font_resource_chars_max_lines(font_resource *r, int height);

#endif // MG_RESOURCE_FONT_H