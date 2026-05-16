
#include <microgame/microgame.h>
#include "dialogue.h"
#include "../scenes/util.h"
#include <math.h>

static int visible = 0;

void check_dialogue_visibility(scene *sc, entity plr, entity npc) {
    vec3 playerPos = get_transform(sc, plr)->pos;
    vec3 npcPos = get_transform(sc, npc)->pos;

    float dist = fabs(LENGTH(SUB(playerPos, npcPos)));

    if (dist < 2) {
        if (key_just_down(M_KEY_E)) {
            set_dialogue_container_visibility(sc, !visible);
            visible = !visible;
        }
    } else {
        set_dialogue_container_visibility(sc, 0);
        visible = 0;
    }
}