
#include "scene_manager.h"
#include "l1.h"

static scene *s;
static entity npc;
static entity player;

void create_scene_1() {
    player = prepare_scene_1();
    npc = get_npc_1();
    s = get_scene_1();
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