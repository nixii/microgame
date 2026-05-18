
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
    s->lightDir = NORMALIZE(vec3_new(1, 3, 1));
    create_ui(s);

    // spawn platforms
    spawn_platform(s, vec3_zero(), vec3_new(2, 1, 2));
    spawn_platform(s, vec3_new(0, 2, 8), vec3_new(1, 0.5, 1));

    for (int i = 0; i < 16; i++)
        spawn_platform(s, vec3_new(0, 3 + i, 12 + 4 * i), vec3_new(0.5, 0.5, 0.5));
    spawn_platform(s, vec3_new(0, 18, 78), vec3_new(1, 0.5, 1));

    spawn_teleport(s, vec3_new(0, 20.5, 78), 3);

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