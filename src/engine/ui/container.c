
#include "microgame/engine/ui/container.h"
#include "microgame/util/color.h"
#include <stdlib.h>
#include <stdio.h>

/*
 * ui_container methods
*/

// create a new ui container w/o data
ui_container *ui_container_empty() {
    ui_container *c = malloc(sizeof(ui_container));

    c->anchor = vec2_zero();
    c->pos = ui_vec_empty();
    c->size = ui_vec_empty();

    c->parent = NULL;
    c->firstChild = NULL;
    c->nextSibling = NULL;

    c->selfType = UI_TYPE_EMPTY;
    c->self = NULL;

    return c;
}

// add a ui container as a child
void ui_container_add_child(ui_container *child, ui_container *parent) {

    // remove the child from any parent
    ui_container_remove_parent(child);

    // make sure parent isn't null
    if (parent == NULL)
        return;

    // add the child
    child->parent = parent;

    // if there is no children in the parent, this is the first one
    if (parent->firstChild == NULL) {
        parent->firstChild = child;
    }

    // otherwise, add this child to the end of the children linked list
    else {
        ui_container *c = parent->firstChild;

        // while there is a next child, do nothing
        while (c->nextSibling != NULL)
            c = c->nextSibling;

        // set the child
        c->nextSibling = child;
    }
}

// remove a ui container as a child
void ui_container_remove_parent(ui_container *child) {
    
    // skip if no parent
    if (child->parent == NULL) return;

    // if the child is the first, simply remove
    if (child->parent->firstChild == child) {
        child->parent->firstChild = NULL;
    }

    // if there are more children, remove the child from the linked list
    else {
        ui_container *c = child->parent->firstChild;

        // while the next sibling is not the current child
        while (c->nextSibling != child)
            c = c->nextSibling;
        
        // set the new next sibling
        c->nextSibling = child->nextSibling;

        // change the next sibling
        child->nextSibling = NULL;
    }

    // remove the parent
    child->parent = NULL;
}

// bind data
void ui_container_bind_type(
    ui_container *container, 
    ui_object_type type, 
    void *data)
{
    
    // if there already is a type then don't 
    if (container->selfType != UI_TYPE_EMPTY)
        return;

    // store the new object
    container->selfType = type;
    container->self = data;
}

// render a ui element
void ui_container_render(ui_container *c, renderer *r, int parentX, int parentY, int parentWidth, int parentHeight) {

    // Calculate self width and height
    int selfWidth = (c->size.scaleX * parentWidth) + c->size.pixelsX;
    int selfHeight = (c->size.scaleY * parentHeight) + c->size.pixelsY;
    int selfX = (c->pos.scaleX * parentWidth) + c->pos.pixelsX + parentX;
    int selfY = (c->pos.scaleY * parentHeight) + c->pos.pixelsY + parentY;

    // Render self based on the type
    switch (c->selfType) {
        case UI_TYPE_EMPTY:
            break;
    #define X_UI(type, name)\
        case type:\
            name##_render(c->self, r, selfX, selfY, selfWidth, selfHeight);\
            break;
        UI_TYPES
    #undef X_UI
    }

    // If there are children, render them all
    if (c->firstChild) {
        ui_container *child = c->firstChild;
        while (child != NULL) {
            ui_container_render(child, r, selfX, selfY, selfWidth, selfHeight);
            child = child->nextSibling;
        }
    }
}

// delete a ui TREE
void ui_container_destroy_tree(ui_container *c) {

    // delete children
    ui_container *child = c->firstChild;
    while (child != NULL) {
        ui_container_destroy_tree(child);
        child = child->nextSibling;
    }

    // delete any ui data types stored
    switch (c->selfType) {
        case UI_TYPE_EMPTY:
            break;
    #define X_UI(type, name)\
        case type:\
            name##_destroy(c->self);\
            break;
        UI_TYPES
    #undef X_UI
    }

    // delete self
    free(c);
}