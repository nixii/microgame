
#include <microgame/microgame.h>
#include <microgame/engine/resource/sound.h>
#include "scene_manager.h"
#include "util.h"
#include "l1.h"
#include "l2.h"
#include "l3.h"
#include "ending.h"
#include <stdlib.h>
#include <stdio.h>

static scene *s;
static entity npc;
static entity player;

static vec3 spawnPos;

static sound travelSound;
static int soundLoaded = 0;

void create_scene_1() {
    player = prepare_scene_1();
    npc = get_npc_1();
    s = get_scene_1();
    spawnPos = get_spawn_pos_1();

    if (!soundLoaded) {
        travelSound = sound_from("testgame1/assets/travel.wav");
    }
}
void create_scene_2() {
    player = prepare_scene_2();
    npc = get_npc_2();
    s = get_scene_2();
    spawnPos = get_spawn_pos_2();

    if (!soundLoaded) {
        travelSound = sound_from("testgame1/assets/travel.wav");
    }
}
void create_scene_3() {
    player = prepare_scene_3();
    npc = get_npc_3();
    s = get_scene_3();
    spawnPos = get_spawn_pos_3();

    if (!soundLoaded) {
        travelSound = sound_from("testgame1/assets/travel.wav");
    }
}
void create_scene_ending() {
    player = prepare_scene_ending();
    npc = NIL_ENTITY;
    s = get_scene_ending();
    spawnPos = get_spawn_pos_ending();
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

scene *handle_scenes() {
    if (get_player() == NIL_ENTITY) return NULL;
    get_transform(s, get_teleport())->rot.y += get_dt() * PI;

    if (get_collider(s, get_teleport())->hit) {
        sound_play(&travelSound);
        switch (get_go_to()) {
            case 1:
                create_scene_1();
                return get_scene_1();
            case 2:
                create_scene_2();
                return get_scene_2();
            case 3:
                create_scene_3();
                return get_scene_3();
            case 4:
                create_scene_ending();
                return get_scene_ending();
            default:
                fprintf(stderr, "no scene %d.\n", get_go_to());
                exit(1);
        }
    }

    return NULL;
}