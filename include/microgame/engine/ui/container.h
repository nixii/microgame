
#ifndef MG_UI_CONTAINER_H
#define MG_UI_CONTAINER_H

#include "microgame/util/uivec.h"
#include "microgame/util/math.h"

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

    // store the other objects
    ui_container *parent;
    ui_container *firstChild;
    ui_container *nextSibling;

    // store its own data
    ui_object_type selfType;
    void *self;
} ui_container;

/*
 * ui_container methods
*/

// create a new ui container w/o data
ui_container ui_container_empty();

// add a ui container as a child
void ui_container_add_child(ui_container *child, ui_container *parent);

// remove a ui contianer as a child
void ui_container_remove_parent(ui_container *child);

// bind data
void ui_container_bind_type(ui_container *container, 
                            ui_object_type type, 
                            void *data);



#endif // MG_UI_CONTAINER_H