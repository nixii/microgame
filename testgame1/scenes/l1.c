
#include <microgame/microgame.h>
#include "util.h"
#include "l1.h"

// the scene thing
static scene *s;

// prepare the first scene
entity prepare_scene_1() {

    // create the scene
    s = scene_new();
    create_ui(s);

    // spawn platforms
    spawn_platform(s, vec3_zero(), vec3_new(2, 1, 2));
    return spawn_player(s, vec3_new(0, 1, 0));
}

// get the scene
scene *get_scene_1() {

    // return the scene
    return s;
}

/*
 * destroy the scene
 */
void destroy_scene_1() {
    scene_destroy(s);
}