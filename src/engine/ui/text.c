
#include "microgame/engine/ui/text.h"
#include "microgame/util/color.h"
#include <string.h>
#include <stdio.h>

/*
 * ui_text functions
*/

// create a new text instance. text can be null.
ui_text *ui_text_new(font_resource *font, const char *text) {

    // create the text
    ui_text *t = malloc(sizeof(ui_text));
    t->autoWrap = 0;

    // load the data
    t->font = font;
    
    // if there is no text, start with a default amount
    if (text == NULL) {
        t->text = NULL;
        t->textLength = 0;
    }

    // if there is text then load that
    else {
        t->text = strdup(text);
        t->textLength = strlen(t->text);
    }

    // return the text
    return t;
}

// destory a ui text
void ui_text_destroy(ui_text *txt) {
    free(txt->text);
    free(txt);
}

// TODO: automatic line breaking
// render ui text
void ui_text_render(ui_text *t, renderer *r, int x, int y, int w, int h) {

    // The target X and Y positions
    int targetX = x;
    int targetY = y;

    // For every character
    for (int i = 0; i < t->textLength; i++) {

        // Get the current character
        char c = t->text[i];

        // Go to the next line
        if (c == '\n') {
            targetX = x;
            targetY += t->font->charHeight + t->font->verticalSpacing;
        }

        // TODO: bounds safely
        // Render the character
        else {

            // if auto wrap is enabled
            if (t->autoWrap) {
                
                // if you are out of space (at least one char a line)
                if (targetX - x > w) {
                    targetX = x;
                    targetY += t->font->charHeight + t->font->verticalSpacing;
                }
            }

            // get the image bounds
            font_bounds bounds = font_resource_get_bounds(t->font, c);

            // draw the character
            renderer_render_image_slice(r, targetX, targetY, bounds.w, bounds.h, bounds.x, bounds.y, t->font->source.width, t->font->source.pixels);
            targetX += t->font->charWidth + t->font->horizontalSpacing;
        }
    }
}