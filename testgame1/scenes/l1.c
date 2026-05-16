
#include <microgame/microgame.h>
#include "util.h"
#include "l1.h"

// the scene thing
static scene *s;
static entity npc;
static vec3 spawnPos1 = {0, 2, 0};

// prepare the first scene
entity prepare_scene_1() {

    // create the scene
    s = scene_new();
    create_ui(s);

    // spawn platforms
    spawn_platform(s, vec3_zero(), vec3_new(2, 1, 2));
    spawn_platform(s, vec3_new(0, 1, 8), vec3_new(0.5, 0.5, 0.5));
    spawn_platform(s, vec3_new(5, 3, 6), vec3_new(1, 0.5, 0.5));

    // create entities
    npc = spawn_npc(s, vec3_new(3, 1, 3), PI / 4);
    return spawn_player(s, spawnPos1);
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

vec3 get_spawn_pos_1() {
    return spawnPos1;
}

/*
 * destroy the scene
 */
void destroy_scene_1() {
    scene_destroy(s);
}