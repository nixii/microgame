
#include "microgame/engine/ui/text.h"
#include "microgame/util/color.h"
#include <string.h>

/*
 * ui_text functions
*/

// create a new text instance. text can be null.
ui_text *ui_text_new(font_resource *font, const char *text) {

    // create the text
    ui_text *t = malloc(sizeof(ui_text));

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

// render ui text
void ui_text_render(ui_text *txt, renderer *r, int x, int y, int w, int h) {
    renderer_render_rectangle(r, 10, 10, 50, 50, rgb(255, 0, 255));
}