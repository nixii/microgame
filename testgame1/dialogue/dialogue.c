
#include "dialogue.h"
#include "../scenes/util.h"
#include <math.h>

void check_dialogue_visibility(scene *sc, entity plr, entity npc) {
    vec3 playerPos = get_transform(sc, plr)->pos;
    vec3 npcPos = get_transform(sc, npc)->pos;

    float dist = fabs(LENGTH(SUB(playerPos, npcPos)));

    if (dist < 2) {
        set_dialogue_container_visibility(sc, 1);
    } else {
        set_dialogue_container_visibility(sc, 0);
    }
}