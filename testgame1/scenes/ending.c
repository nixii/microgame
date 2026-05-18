

#include <microgame/microgame.h>
#include "util.h"
#include "ending.h"

// the scene thing
static scene *s;

// prepare the first scene
entity prepare_scene_ending() {

    // create the scene
    s = scene_new();

    // ending winner UI!
    create_end_ui(s);

    return NIL_ENTITY;
}

// get the scene
scene *get_scene_ending() {

    // return the scene
    return s;
}

vec3 get_spawn_pos_ending() {
    return vec3_zero();
}

/*
 * destroy the scene
 */
void destroy_scene_ending() {
    scene_destroy(s);
}