
#include <microgame/microgame.h>
#include "util.h"
#include "l1.h"

// the scene thing
static scene *s;
static entity npc;

// prepare the first scene
entity prepare_scene_1() {

    // create the scene
    s = scene_new();
    create_ui(s);

    // spawn platforms
    spawn_platform(s, vec3_zero(), vec3_new(2, 1, 2));
    spawn_platform(s, vec3_new(0, 1, 8), vec3_new(0.5, 0.5, 0.5));

    // create entities
    npc = spawn_npc(s, vec3_new(3, 1, 3), PI / 4);
    return spawn_player(s, vec3_new(0, 1, 0));
}

// get the npc
entity get_npc_1() {
    return npc;
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