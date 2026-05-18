
#include <microgame/microgame.h>
#include "scenes/scene_manager.h"
#include "movement/movement.h"
#include "dialogue/dialogue.h"
#include "scenes/util.h"
#include <stdio.h>

// window settings
#define WIDTH 1200
#define HEIGHT 800
#define FPS 60
#define TITLE "Platformer!"

// main loop!
int main(void) {

    // create the game
    microgame *game = game_new(WIDTH, HEIGHT, FPS, TITLE);
    disable_mouse();

    // load the scenes
    // create_scene_1();
    create_scene_3();

    // get objects
    entity player = get_player();
    entity npc = get_npc();

    // set the main scene
    scene *sc = get_scene();
    game_set_scene(game, sc);
    set_dialogue_container_visibility(sc, 1);

    // while the game is running
    while (game_still_running(game)) {
        update_movement(sc, player);
        if (player != NIL_ENTITY) check_dialogue_visibility(sc, player, npc);
        scene *goToThis = handle_scenes();

        if (goToThis != NULL) {
            sc = goToThis;
            game_set_scene(game, sc);
            player = get_player();
            npc = get_npc();
            continue;
        }

        game_update(game);
        rescue(sc, player);
        if (player != NIL_ENTITY) {
            float targetFov = (PI / 2) + LENGTH(MUL_COMPONENTS(get_velocity(sc, player)->velocity, vec3_new(1, 0, 1))) / 20;
            sc->camera.fov += (targetFov - sc->camera.fov) * 0.2f;
        }
    }

    // destroy the game
    game_destroy(game);
}