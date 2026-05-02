
#ifndef MG_UI_CONTAINER_H
#define MG_UI_CONTAINER_H

#include "microgame/renderer/renderer.h"
#include "microgame/engine/ui/image.h"
#include "microgame/engine/ui/rect.h"
#include "microgame/engine/ui/text.h"
#include "microgame/util/uivec.h"
#include "microgame/util/math.h"

// the different kinds of UI objects
typedef enum ui_object_type {
    UI_TYPE_EMPTY,
    UI_TYPE_RECT,
    UI_TYPE_IMAGE,
    UI_TYPE_TEXT,
} ui_object_type;

// X macro for each ui type
#define UI_TYPES\
    X_UI(UI_TYPE_RECT, ui_rect)\
    X_UI(UI_TYPE_IMAGE, ui_image)\
    X_UI(UI_TYPE_TEXT, ui_text)

// the container
typedef struct ui_container {

    // anchor, position
    vec2 anchor;
    ui_vec pos;
    ui_vec size;

    // store the other objects
    struct ui_container *parent;
    struct ui_container *firstChild;
    struct ui_container *nextSibling;

    // store its own data
    ui_object_type selfType;
    void *self;

    // tint the ui
    color overlay;

    // button events
    void (*onHover)(struct ui_container *self, int hoverActivated);
    void (*onClick)(struct ui_container *self, int down);

    // is it hovered
    int hovered;
    int pressed;
} ui_container;

/*
 * ui_container methods
*/

// create a new ui container w/o data
ui_container *ui_container_empty();

// add a ui container as a child
void ui_container_set_parent(ui_container *child, ui_container *parent);

// remove a ui contianer as a child
void ui_container_remove_parent(ui_container *child);

// update the ui container
void ui_container_update(
    ui_container *container, 
    vec2 mousePos, 
    int leftPress, 
    int parentX,
    int parentY,
    int parentWidth,
    int parentHeight);

// bind data
void ui_container_bind_type(
    ui_container *container, 
    ui_object_type type, 
    void *data);

// render a ui element
void ui_container_render(
    ui_container *c, 
    renderer *r, 
    int parentX, 
    int parentY, 
    int parentWidth, 
    int parentHeight);

// delete a ui TREE
void ui_container_destroy_tree(ui_container *c);

#endif // MG_UI_CONTAINER_H