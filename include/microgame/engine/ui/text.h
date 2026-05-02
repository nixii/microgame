
#ifndef MG_UI_TEXT_H
#define MG_UI_TEXT_H

#include "microgame/engine/resource/font.h"
#include "microgame/renderer/renderer.h"
#include <stddef.h>

// create the type
typedef struct ui_text {
    font_resource *font;
    char *text;
    int textLength;
    int autoWrap;
} ui_text;

/*
 * ui_text functions
*/

// create a new text instance. text can be null.
ui_text *ui_text_new(font_resource *font, const char *text);

// destory a ui text
void ui_text_destroy(ui_text *txt);

// render ui text
void ui_text_render(
    ui_text *txt, 
    renderer *r, 
    int x, 
    int y, 
    int w, 
    int h, 
    color overlay);

#endif // MG_UI_TEXT_H