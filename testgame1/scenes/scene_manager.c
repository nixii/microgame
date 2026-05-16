
#include <microgame/microgame.h>
#include "scene_manager.h"
#include "util.h"
#include "l1.h"

static scene *s;
static entity npc;
static entity player;

static vec3 spawnPos;

void create_scene_1() {
    player = prepare_scene_1();
    npc = get_npc_1();
    s = get_scene_1();
    spawnPos = get_spawn_pos_1();
}
void create_scene_2() {

}
void create_scene_3() {

}

scene *get_scene() {
    return s;
}
entity get_npc() {
    return npc;
}
entity get_player() {
    return player;
}

void destroy_all_scenes() {
    destroy_scene_1();
}

vec3 get_spawn_pos() {
    return spawnPos;
}

void handle_scenes() {
    get_transform(s, get_teleport())->rot.y += get_dt() * PI;
}