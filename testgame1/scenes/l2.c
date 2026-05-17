
#include <microgame/microgame.h>
#include "util.h"
#include "l2.h"
#include <stdio.h>

// the scene thing
static scene *s;
static entity npc;
static vec3 spawnPos = {0, 2, 0};

// prepare the first scene
entity prepare_scene_2() {

    // create the scene
    s = scene_new();
    create_ui(s);

    // spawn platforms
    spawn_platform(s, vec3_zero(), vec3_new(2, 1, 2));
    spawn_platform(s, vec3_new(0, 2, 8), vec3_new(1, 0.5, 1));

    spawn_teleport(s, vec3_new(25, 10.5, 5), 2);

    // create entities
    npc = spawn_npc(s, vec3_new(3, 1, 3), PI / 4);
    return spawn_player(s, spawnPos);
}

// get the npc
entity get_npc_2() {
    return npc;
}

// get the scene
scene *get_scene_2() {

    // return the scene
    return s;
}

vec3 get_spawn_pos_2() {
    return spawnPos;
}

/*
 * destroy the scene
 */
void destroy_scene_2() {
    scene_destroy(s);
}