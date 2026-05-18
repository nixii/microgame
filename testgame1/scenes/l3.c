
#include <microgame/microgame.h>
#include "util.h"
#include "l3.h"

// the scene thing
static scene *s;
static entity npc;
static vec3 spawnPos = {0, 2, 0};

// prepare the first scene
entity prepare_scene_3() {

    // create the scene
    s = scene_new();
    create_ui(s);

    // spawn platforms
    spawn_platform(s, vec3_zero(), vec3_new(2, 1, 2));

    for (int i = 0; i < 17; i++) {
        float cs = cosf(i * PI / 2);
        float si = sinf(i * PI / 2);
        spawn_platform(s, vec3_new(12 + cs * 3, 1.5*i + 1, si * 3), vec3_new(0.3, 0.5, 0.3));
    }

    spawn_platform(s, vec3_new(26, 26, 0), vec3_new(1, 1, 1));
    spawn_platform(s, vec3_new(36, 27, 0), vec3_new(1, 1, 1));
    
    spawn_platform(s, vec3_new(46, 28, 0), vec3_new(1, 1, 1));
    spawn_platform(s, vec3_new(49, 29, 0), vec3_new(0.5, 1, 10));
    spawn_platform(s, vec3_new(49, 32, 0), vec3_new(0.5, 1, 10));
    spawn_platform(s, vec3_new(52, 28, 0), vec3_new(1, 1, 1));

    spawn_teleport(s, vec3_new(52, 30, 0), 4);

    // create entities
    npc = spawn_npc(s, vec3_new(3, 1, 3), PI / 4);
    return spawn_player(s, spawnPos);
}

// get the npc
entity get_npc_3() {
    return npc;
}

// get the scene
scene *get_scene_3() {

    // return the scene
    return s;
}

vec3 get_spawn_pos_3() {
    return spawnPos;
}

/*
 * destroy the scene
 */
void destroy_scene_3() {
    scene_destroy(s);
}