
#ifndef MG_UI_CONTAINER_H
#define MG_UI_CONTAINER_H

#include "microgame/util/uivec.h"
#include "microgame/util/math.h"
#include "microgame/renderer/renderer.h"

// the different kinds of UI objects
typedef enum ui_object_type {
    UI_TYPE_EMPTY,
    UI_TYPE_CONTAINER,
    UI_TYPE_RECT,
    UI_TYPE_IMAGE,
    UI_TYPE_RECT_BUTTON,
    UI_TYPE_IMAGE_BUTTON,
} ui_object_type;

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
} ui_container;

/*
 * ui_container methods
*/

// create a new ui container w/o data
ui_container *ui_container_empty();

// add a ui container as a child
void ui_container_add_child(ui_container *child, ui_container *parent);

// remove a ui contianer as a child
void ui_container_remove_parent(ui_container *child);

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