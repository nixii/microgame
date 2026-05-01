
#include "microgame/ui/container.h"

/*
 * ui_container methods
*/

// create a new ui container w/o data
ui_container ui_container_empty() {
    return (ui_container){
        .anchor = vec2_zero(),
        .pos = ui_vec_empty(),
        
        .parent = NULL,
        .firstChild = NULL,
        .nextSibling = NULL,

        .ui_object_type = UI_TYPE_EMPTY,
        .self = NULL
    };
}

// add a ui container as a child
void ui_container_add_child(ui_container *child, ui_container *parent) {

    // remove the child from any parent
    ui_container_remove_parent(child)

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