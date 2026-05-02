
#include "microgame/engine/resource/font.h"
#include <string.h>
#include <stddef.h>

/*
 * font resource functions
*/

// create a new font resource
font_resource font_resource_from(
    const char *path, 
    int charWidth, 
    int charHeight, 
    int horizontalSpacing, 
    int verticalSpacing)
{

    // create the font
    font_resource r = { 0 };

    // load the image
    // TODO: manage errors
    image_resource img = image_resource_from(path);

    // load the struct data
    r.charWidth = charWidth;
    r.charHeight = charHeight;
    r.horizontalSpacing = horizontalSpacing;
    r.verticalSpacing = verticalSpacing;
    r.source = img;

    // return the font
    return r;
}

// destroy a font resource
void font_resource_destroy(font_resource *r) {

    // destroy the image
    image_resource_destroy(&r->source);

    // set everything to zero
    r->charWidth = -1;
    r->charHeight = -1;
    r->horizontalSpacing = -1;
    r->verticalSpacing = -1;
}

// get the bounds of a letter
font_bounds font_resource_get_bounds(font_resource *r, char c) {

    // get the index of the first character (32 is space and the start.)
    int idx = (int)c - ' '; 

    // simple!
    return (font_bounds){
        .w = r->charWidth,
        .h = r->charHeight,
        .x = idx * r->charWidth,
        .y = 0 // default to 0 for now
    };
}

// get the width of text in pixels. assumes it is at least 1 length.
int font_resource_get_width(font_resource *r, const char *text) {

    // longest line so far
    int longest = 0, current = 0;
    
    // continue for each char
    for (const char *c = text; *c != '\0'; c++) {

        // if it is a newline then continue as such
        if (*c == '\n') {
            if (current > longest)
                longest = current;
            current = 0;
        }

        // simply increment
        else {
            current++;
        }
    }

    // return the new length!
    return longest * (r->charWidth + r->horizontalSpacing) - r->horizontalSpacing;
}

// get the height of text in pixels (looks at newlines)
int font_resource_get_height(font_resource *r, const char *text) {

    // the amount of lines
    int lines = 0;

    // while there are characters
    for (const char *character = text; *character != '\0'; character++) {
        if (*character == '\n')
            lines++;
    }

    // return the new height
    return lines * (r->charHeight + r->verticalSpacing) - r->verticalSpacing;
}

// get the amount of characters that fit in a line of specified width
int font_resource_chars_per_line(font_resource *r, int width) {
    return width / (r->charWidth + r->horizontalSpacing); // close enough...
}

// get the amount of columns that fit in a line of specified height
int font_resource_chars_max_lines(font_resource *r, int height) {
    return height / (r->charHeight + r->verticalSpacing); // close enough!
}